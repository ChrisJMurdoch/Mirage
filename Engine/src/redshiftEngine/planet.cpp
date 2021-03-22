
#include "redshiftEngine/planet.hpp"

#include "utility/math.hpp"
#include "generate/noise.hpp"

#include <math.h>
#include <thread>
#include <list>
#include <unordered_map>

// VERTEX DATA
static int const CUBE_FACES = 6, TRIANGLES_PER_QUAD = 2;

// DOMAIN WARPING
static float const WARP_MAGNITUDE = 0.15f, WARP_PERIOD = 0.25f;

// NOISE
static float const MAGNITUDE = 0.5f;
static float const LAND_MAGNITUDE = 0.5f, LAND_PERIOD = 0.5f;
static float const FINE_MAGNITUDE = 0.005f, FINE_PERIOD = 0.01f;

// MOUNTAIN MASK
static float const MMASK_PERIOD = 0.25f, MMASK_BIAS = 1.00f;
static int const MMASK_OCTAVES = 2, MMASK_PASSES = 3;

Planet::Planet(int edgeVertices, int octaves, int seed) : octaves(octaves), seed(seed), vertices( edgeVertices*edgeVertices*CUBE_FACES ), indices( (edgeVertices-1)*(edgeVertices-1)*TRIANGLES_PER_QUAD*CUBE_FACES )
{
    generateCube(edgeVertices);
    setPositions(16);
    fixNormals();
    setColours(16);
}

void Planet::generateCube(int edgeVertices)
{
    if (edgeVertices < 2)
        throw std::exception("Mesh cannot have < 2 edge vertices.");

    // Dimension variable: lines -> squares -> cubes
    int edgeLines = edgeVertices-1;

    // Array strides
    int verticesFaceStride = edgeVertices * edgeVertices;
    int indicesFaceStride = edgeLines * edgeLines * TRIANGLES_PER_QUAD;

    // Space between vertices
    float space = 1.0f / edgeLines;

    // Generate faces
    generatePlane(edgeVertices, verticesFaceStride*0, indicesFaceStride*0, Coord{ 0.5f,-0.5f,-0.5f }, Coord{ 0,space,space }, true); // X+
    generatePlane(edgeVertices, verticesFaceStride*1, indicesFaceStride*1, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ 0,space,space }, false); // X-
    generatePlane(edgeVertices, verticesFaceStride*2, indicesFaceStride*2, Coord{ -0.5f, 0.5f,-0.5f }, Coord{ space,0,space }, true); // Y+
    generatePlane(edgeVertices, verticesFaceStride*3, indicesFaceStride*3, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ space,0,space }, false); // Y-
    generatePlane(edgeVertices, verticesFaceStride*4, indicesFaceStride*4, Coord{ -0.5f,-0.5f, 0.5f }, Coord{ space,space,0 }, false); // Z+
    generatePlane(edgeVertices, verticesFaceStride*5, indicesFaceStride*5, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ space,space,0 }, true); // Z-
}

static inline float curveAdjust(float x)
{
    return (float)tan(0.5*M_PI*x) / 2;
}

void Planet::generatePlane(int edgeVertices, int verticesOffset, int indicesOffset, Planet::Coord origin, Planet::Coord delta, bool wind)
{
    // Shorthand edge-vertices and edge-lines
    int ev = edgeVertices, el = edgeVertices-1;

    // Generate vertices on 2D plane a*b
    for (int a=0; a<ev; a++)
    {
        for (int b=0; b<ev; b++)
        {
            // Calculate vertex index
            int i = (a*ev) + b + verticesOffset;

            VirtualVector position = vertices.position(i);
            position.setX(curveAdjust(origin.x + a*delta.x));
            position.setY(curveAdjust(origin.y + b*delta.y));
            position.setZ(curveAdjust((delta.z == 0) ? (origin.z) : (delta.x==0) ? (origin.z + a*delta.z) : (origin.z + b*delta.z)));

            VirtualVector normal = vertices.normal(i);
            normal.set(position.getX(), position.getY(), position.getZ());
            normal.normalise();

            VirtualVector colour = vertices.colour(i);
            colour.set(1, 1, 1);
        }
    }

    // Alternate quad splitting direction
    bool toggle = false;

    // Generate indices
    for (int y=0; y<el; y++)
    {
        for (int x=0; x<el; x++)
        {
            // Calculate index for quad
            int i = (((y*el) + x) * TRIANGLES_PER_QUAD) + indicesOffset;

            // Calulate indices for vertices
            int xo=x, yo=y, xp=x+1, yp=y+1;
            int topleft = (yo*ev)+xo,
                topright = (yo*ev)+xp,
                botright = (yp*ev)+xp,
                botleft = (yp*ev)+xo;

            int a = wind ? topright : botleft, b = wind ? botleft : topright;

            indices[i+0][0] = verticesOffset + topleft;
            indices[i+0][1] = verticesOffset + a;
            indices[i+0][2] = verticesOffset + botright;

            indices[i+1][0] = verticesOffset + botright;
            indices[i+1][1] = verticesOffset + b;
            indices[i+1][2] = verticesOffset + topleft;
        }
    }
}

VertexArray const &Planet::getVertices() const
{
    return vertices;
}

IndexArray const &Planet::getIndices() const
{
    return indices;
}

static inline float ridge(float input)
{
    return 1 - abs(input)*2; // (-1,1) [mode=0] -> (-1,1) [mode=1]
}

static inline float createMask(float noise, int smoothPasses, float bias)
{
    float mask = (noise+1)/2; // (-1,1) [mode=0] -> (0,1) [mode=0.5]
    mask *= bias;
    for (int i=0; i<smoothPasses; i++)
        mask = math::smooth(mask); // Standard deviation ++
    return mask;
}

void Planet::setPosition(VirtualVector position)
{
    // Create sphere for sample coords
    position.normalise();

    // Get vector components
    float x=position.getX(), y=position.getY(), z=position.getZ();

    // Seed hashes
    int seed = this->seed;

    // Domain warp
    x += WARP_MAGNITUDE * noise::perlinSample(x, y, z, WARP_PERIOD, seed++);
    y += WARP_MAGNITUDE * noise::perlinSample(x, y, z, WARP_PERIOD, seed++);
    z += WARP_MAGNITUDE * noise::perlinSample(x, y, z, WARP_PERIOD, seed++);

    // LAND SHAPE

    // Generate noise values
    float land_noise = noise::fractalSample(x, y, z, LAND_PERIOD, octaves, seed++);
    float fine_noise = noise::fractalSample(x, y, z, FINE_PERIOD, 3, seed++);

    // Generate noise masks
    float mmask = createMask(noise::fractalSample(x, y, z, MMASK_PERIOD, MMASK_OCTAVES, seed++), MMASK_PASSES, MMASK_BIAS);

    // Mix values
    float value = (mmask*land_noise*LAND_MAGNITUDE) + (fine_noise*FINE_MAGNITUDE);

    // Raise land
    value += 0.01f;

    // Set height
    float height = 1 + value*MAGNITUDE;
    position.normalise(height);
}

// Grid-stride allocation
void Planet::setPositionsThread(int offset, int stride)
{
    for (int i=offset; i<vertices.getNVertices(); i+=stride)
    {
        setPosition(vertices.position(i));
    }
}

void Planet::setPositions(int threads)
{
    // Allocate threads
    std::list<std::thread> threadList;
    for (int i=0; i<threads; i++)
    {
        threadList.push_back( std::thread([this, i, threads] { setPositionsThread(i, threads); }) );
    }

    // Wait for threads to finish
    for (std::thread &thread : threadList)
    {
        thread.join();
    }
}

/* Calculate normals for each vertex by averaging normals of composite triangles */
void Planet::fixNormals()
{
    std::unordered_map<int, int> occurrences;
    std::unordered_map<int, glm::vec3> normals;

    // Every triangle
    for (int i=0; i<indices.getNTriangles(); i++)
    {
        // Get tri
        unsigned int *tri = indices[i];
        glm::vec3 a = vertices.position(tri[0]).asVec3(),
            b = vertices.position(tri[1]).asVec3(),
            c = vertices.position(tri[2]).asVec3();

        // Calculate normal
        glm::vec3 normal = glm::cross((c-b), (a-b));

        // Increment occurrences
        occurrences[tri[0]]++;
        occurrences[tri[1]]++;
        occurrences[tri[2]]++;

        // Add normal to totals
        normals[tri[0]] += normal;
        normals[tri[1]] += normal;
        normals[tri[2]] += normal;
    }

    // Set vertices
    for (int i=0; i<vertices.getNVertices(); i++)
    {
        glm::vec3 normal = glm::normalize(normals[i] / (float)occurrences[i]);
        vertices.normal(i).set(normal.x, normal.y, normal.z);
    }
}

void Planet::setColour(VirtualVector position, VirtualVector normal, VirtualVector colour)
{
    glm::vec3 position_vec = glm::normalize(position.asVec3());
    glm::vec3 normal_vec = glm::normalize(normal.asVec3());

    float flatness = glm::dot(position_vec, normal_vec);
    flatness = pow(flatness, 24.0f);

    glm::vec3 const SAND(0.9f, 0.9f, 0.7f);
    glm::vec3 const GRASS(0.3f, 0.6f, 0.1f);
    glm::vec3 const STONE(0.3f, 0.3f, 0.3f);
    glm::vec3 const SNOW(1.0f, 1.0f, 1.0f);

    float const height = position.getMagnitude();
    glm::vec3 colour_vec;
    if (height<1.004f)
        colour_vec = SAND;
    else if (height<1.02f)
        colour_vec = (GRASS*flatness) + (STONE*(1-flatness));
    else if (height<1.03f)
        colour_vec = STONE;
    else
        colour_vec = flatness>0.5f ? SNOW : STONE;

    colour.set(colour_vec.x, colour_vec.y, colour_vec.z);
}

// Grid-stride allocation
void Planet::setColoursThread(int offset, int stride)
{
    for (int i=offset; i<vertices.getNVertices(); i+=stride)
    {
        setColour(vertices.position(i), vertices.normal(i), vertices.colour(i));
    }
}

void Planet::setColours(int threads)
{
    // Allocate threads
    std::list<std::thread> threadList;
    for (int i=0; i<threads; i++)
    {
        threadList.push_back( std::thread([this, i, threads] { setColoursThread(i, threads); }) );
    }

    // Wait for threads to finish
    for (std::thread &thread : threadList)
    {
        thread.join();
    }
}
