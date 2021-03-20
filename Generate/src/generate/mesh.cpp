
#include "generate/mesh.hpp"

#include "model/virtualVector.hpp"
#include "generate/noise.hpp"
#include "utility/math.hpp"
#include "engine/planet.hpp"

#include "glm/glm.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <unordered_map>
#include <thread>
#include <list>

static int const CUBE_FACES = 6, TRIANGLES_PER_QUAD = 2;

int mesh::vertexCount(int edgeVertices)
{
    return edgeVertices * edgeVertices * CUBE_FACES;
}

int mesh::triangleCount(int edgeVertices)
{
    return (edgeVertices-1) * (edgeVertices-1) * TRIANGLES_PER_QUAD * CUBE_FACES;
}

static inline float curveAdjust(float x)
{
    return (float)tan(0.5*M_PI*x) / 2;
}

static void generatePlane(int edgeVertices, VertexArray &vertexArray, IndexArray &indexArray, int verticesOffset, int indicesOffset, mesh::Coord origin, mesh::Coord delta, bool wind)
{
    // Shorthand edge-vertices and edge-lines
    int ev = edgeVertices, el = edgeVertices-1;

    // Generate vertices on 2D plane axb
    for (int a=0; a<ev; a++)
    {
        for (int b=0; b<ev; b++)
        {
            // Calculate vertex index
            int i = (a*ev) + b + verticesOffset;

            VirtualVector position = vertexArray.position(i);
            position.setX( curveAdjust( origin.x + a*delta.x ) );
            position.setY( curveAdjust( origin.y + b*delta.y ) );
            position.setZ( curveAdjust( (delta.z == 0) ? (origin.z) : (delta.x==0) ? (origin.z + a*delta.z) : (origin.z + b*delta.z) ) );

            VirtualVector normal = vertexArray.normal (i);
            normal.set( position.getX(), position.getY(), position.getZ() );
            normal.normalise();

            VirtualVector colour = vertexArray.colour(i);
            colour.set( 1, 1, 1 );
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

            indexArray[i+0][0] = verticesOffset + topleft;
            indexArray[i+0][1] = verticesOffset + a;
            indexArray[i+0][2] = verticesOffset + botright;

            indexArray[i+1][0] = verticesOffset + botright;
            indexArray[i+1][1] = verticesOffset + b;
            indexArray[i+1][2] = verticesOffset + topleft;
        }
    }
}

void mesh::generateCube(int edgeVertices, VertexArray &vertexArray, IndexArray &indexArray)
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
    generatePlane( edgeVertices, vertexArray, indexArray, verticesFaceStride*0, indicesFaceStride*0, Coord{  0.5f,-0.5f,-0.5f }, Coord{ 0,space,space }, true  ); // X+
    generatePlane( edgeVertices, vertexArray, indexArray, verticesFaceStride*1, indicesFaceStride*1, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ 0,space,space }, false ); // X-
    generatePlane( edgeVertices, vertexArray, indexArray, verticesFaceStride*2, indicesFaceStride*2, Coord{ -0.5f, 0.5f,-0.5f }, Coord{ space,0,space }, true  ); // Y+
    generatePlane( edgeVertices, vertexArray, indexArray, verticesFaceStride*3, indicesFaceStride*3, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ space,0,space }, false ); // Y-
    generatePlane( edgeVertices, vertexArray, indexArray, verticesFaceStride*4, indicesFaceStride*4, Coord{ -0.5f,-0.5f, 0.5f }, Coord{ space,space,0 }, false ); // Z+
    generatePlane( edgeVertices, vertexArray, indexArray, verticesFaceStride*5, indicesFaceStride*5, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ space,space,0 }, true  ); // Z-
}

// Grid-stride allocation
static void setPositionsThread(VertexArray *vertexArray, mesh::Shape *shape, int offset, int stride)
{
    for (int i=offset; i<vertexArray->getNVertices(); i+=stride)
    {
        shape->setPosition(vertexArray->position(i));
    }
}

void mesh::setPositions(VertexArray &vertexArray, Shape *shape, int threads)
{
    // Allocate threads
    std::list<std::thread> threadList;
    for (int i=0; i<threads; i++)
    {
        threadList.push_back(std::thread(setPositionsThread, &vertexArray, shape, i, threads));
    }

    // Wait for threads to finish
    for (std::thread &thread : threadList)
    {
        thread.join();
    }
}

// Grid-stride allocation
static void setColoursThread(VertexArray *vertexArray, mesh::Shape *shape, int offset, int stride)
{
    for (int i=offset; i<vertexArray->getNVertices(); i+=stride)
    {
        shape->setColour(vertexArray->position(i), vertexArray->normal(i), vertexArray->colour(i));
    }
}

void mesh::setColours(VertexArray &vertexArray, Shape *shape, int threads)
{
    // Allocate threads
    std::list<std::thread> threadList;
    for (int i=0; i<threads; i++)
    {
        threadList.push_back(std::thread(setColoursThread, &vertexArray, shape, i, threads));
    }

    // Wait for threads to finish
    for (std::thread &thread : threadList)
    {
        thread.join();
    }
}

/* Calculate normals for each vertex by averaging normals of composite triangles */
void mesh::fixNormals(VertexArray &vertexArray, IndexArray &indexArray)
{
    std::unordered_map<int, int> occurrences;
    std::unordered_map<int, glm::vec3> normals;

    // Every triangle
    for (int i=0; i<indexArray.getNTriangles(); i++)
    {
        // Get tri
        unsigned int *tri = indexArray[i];
        glm::vec3 a = vertexArray.position(tri[0]).asVec3(),
                  b = vertexArray.position(tri[1]).asVec3(),
                  c = vertexArray.position(tri[2]).asVec3();

        // Calculate normal
        glm::vec3 normal = glm::cross( (c-b), (a-b) );
        
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
    for (int i=0; i<vertexArray.getNVertices(); i++)
    {
        glm::vec3 normal = glm::normalize( normals[i] / (float)occurrences[i]);
        vertexArray.normal(i).set(normal.x, normal.y, normal.z);
    }
}
