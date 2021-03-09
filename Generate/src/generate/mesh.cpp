
#include "generate/mesh.h"

#include "model/virtualVector.h"
#include "generate/noise.h"
#include "utility/math.h"

#include "glm/glm.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <unordered_map>
#include <thread>
#include <list>

const int CUBE_FACES = 6, TRIANGLES_PER_QUAD = 2;

int mesh::vertexCount(int edgeVertices)
{
    return edgeVertices * edgeVertices * CUBE_FACES;
}

int mesh::triangleCount(int edgeVertices)
{
    return (edgeVertices-1) * (edgeVertices-1) * TRIANGLES_PER_QUAD * CUBE_FACES;
}

void generatePlane(int edgeVertices, VertexArray &vertexArray, IndexArray &indexArray, int verticesOffset, int indicesOffset, mesh::Coord origin, mesh::Coord delta, bool wind);

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

inline float curveAdjust(float x)
{
    return (float)tan(0.5*M_PI*x) / 2;
}

void generatePlane(int edgeVertices, VertexArray &vertexArray, IndexArray &indexArray, int verticesOffset, int indicesOffset, mesh::Coord origin, mesh::Coord delta, bool wind)
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

// WARPING
static float const WARP_MAGNITUDE = 0.15f, WARP_PERIOD = 0.25f;

// NOISE
static float const NOISE_MAGNITUDE = 0.15f, NOISE_PERIOD = 0.2f;

// MASKS
static float const MMASK_PERIOD = 0.25f, MMASK_SMOOTH_PASSES = 4;

// Grid-stride allocation
void planetThread(VertexArray *vertexArray, int octaves, int offset, int stride)
{
    for (int i=offset; i<vertexArray->getNVertices(); i+=stride)
    {
        // Get vectors
        VirtualVector position = vertexArray->position(i);
        VirtualVector colour = vertexArray->colour(i);

        // Create sphere for sample coords
        position.normalise();

        // Domain warp
        float warpX = WARP_MAGNITUDE * noise::perlinSample(position.getX(), position.getY(), position.getZ(), WARP_PERIOD);
        float warpY = WARP_MAGNITUDE * noise::perlinSample(position.getY(), position.getZ(), position.getX(), WARP_PERIOD);
        float warpZ = WARP_MAGNITUDE * noise::perlinSample(position.getZ(), position.getX(), position.getY(), WARP_PERIOD);

        // Generate noise
        float landNoise = noise::fractalSample(position.getX()+warpX, position.getY()+warpY, position.getZ()+warpZ, NOISE_PERIOD, octaves) * 0.25f;
        float mountainNoise = noise::fractalSample(position.getX()+warpX+100, position.getY()+warpY+100, position.getZ()+warpZ+100, NOISE_PERIOD, octaves) * 0.75f;

        // Generate masks
        float mountainMask = noise::fractalSample(position.getX()+warpX, position.getY()+warpY, position.getZ()+warpZ, MMASK_PERIOD, 1);
        mountainMask = (mountainMask+1)/2;

        for (int i=0; i<MMASK_SMOOTH_PASSES; i++)
            mountainMask = math::smooth(mountainMask);

        // MIX
        float value = (mountainMask*mountainNoise) + landNoise;

        // Set new vector magnitude
        position.normalise( 1 + (value*NOISE_MAGNITUDE) );

        if (value>0.13)
            colour.set(1.0f, 1.0f, 1.0f);
        else if (value>0.08)
            colour.set(0.5f, 0.5f, 0.5f);
        else if(value>0.015)
            colour.set(0.3f, 0.6f, 0.1f);
        else
            colour.set(0.85f, 0.85f, 0.7f);
    }
}

void mesh::planet(VertexArray &vertexArray, int octaves, int threads)
{
    // Allocate threads
    std::list<std::thread> threadList;
    for (int i=0; i<threads; i++)
    {
        threadList.push_back( std::thread(planetThread, &vertexArray, octaves, i, threads) );
    }
    for (std::thread &thread : threadList)
    {
        thread.join();
    }
}

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
        
        // Add to maps
        occurrences[tri[0]]++;
        occurrences[tri[1]]++;
        occurrences[tri[2]]++;
        normals[tri[0]] += normal;
        normals[tri[1]] += normal;
        normals[tri[2]] += normal;
    }

    // Set vertices
    for (int i=0; i<vertexArray.getNVertices(); i++)
    {
        int occurrence = occurrences[i];
        glm::vec3 normal = glm::normalize( normals[i] / (float)occurrence );
        vertexArray.normal(i).set(normal.x, normal.y, normal.z);
    }
}