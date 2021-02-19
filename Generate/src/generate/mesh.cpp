
#include "generate\mesh.h"

#include "model\virtualVector.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <exception>

const int CUBE_FACES = 6, TRIANGLES_PER_QUAD = 2;

Mesh::Mesh(int edgeVertices) : indexArray( (edgeVertices-1) * (edgeVertices-1) * TRIANGLES_PER_QUAD * CUBE_FACES), vertexArray( edgeVertices * edgeVertices * CUBE_FACES)
{
    if (edgeVertices < 2)
        throw std::exception("Mesh cannot have < 2 edge vertices.");
    int edgeLines = edgeVertices-1;

    // Calculate array strides
    int verticesFaceStride = edgeVertices*edgeVertices;
    int indicesFaceStride = edgeLines*edgeLines*TRIANGLES_PER_QUAD;
    float space = 1.0f / edgeLines; // Space between vertices

    // Faces
    generatePlane( edgeVertices, verticesFaceStride*0, indicesFaceStride*0, Coord{  0.5f,-0.5f,-0.5f }, Coord{ 0,space,space } ); // X+
    generatePlane( edgeVertices, verticesFaceStride*1, indicesFaceStride*1, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ 0,space,space } ); // X-
    generatePlane( edgeVertices, verticesFaceStride*2, indicesFaceStride*2, Coord{ -0.5f, 0.5f,-0.5f }, Coord{ space,0,space } ); // Y+
    generatePlane( edgeVertices, verticesFaceStride*3, indicesFaceStride*3, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ space,0,space } ); // Y-
    generatePlane( edgeVertices, verticesFaceStride*4, indicesFaceStride*4, Coord{ -0.5f,-0.5f, 0.5f }, Coord{ space,space,0 } ); // Z+
    generatePlane( edgeVertices, verticesFaceStride*5, indicesFaceStride*5, Coord{ -0.5f,-0.5f,-0.5f }, Coord{ space,space,0 } ); // Z-
    
    std::cout << "+Mesh - Vertices: " << vertexArray.getNVertices() << ", Indices: " << indexArray.getNIndices() << std::endl;
}

void Mesh::generatePlane(int edgeVertices, int verticesOffset, int indicesOffset, Coord origin, Coord delta)
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
            position.setX(origin.x + a*delta.x);
            position.setY(origin.y + b*delta.y);
            position.setZ( (delta.z == 0) ? (origin.z) : (delta.x==0) ? (origin.z + a*delta.z) : (origin.z + b*delta.z) );

            VirtualVector normal = vertexArray.normal (i);
            normal.set( position.getX(), position.getY(), position.getZ() );
            normal.normalise();

            VirtualVector colour = vertexArray.colour(i);
            colour.setX(float(rand()) / float((RAND_MAX)));
            colour.setY(float(rand()) / float((RAND_MAX)));
            colour.setZ(float(rand()) / float((RAND_MAX)));
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

            // Triangle 1
            indexArray[i][0] = verticesOffset+topleft;
            indexArray[i][1] = verticesOffset+topright;
            indexArray[i][2] = verticesOffset+botright;

            // Triangle 2
            indexArray[i+1][0] = verticesOffset+botright;
            indexArray[i+1][1] = verticesOffset+botleft;
            indexArray[i+1][2] = verticesOffset+topleft;
        }
    }
}

void Mesh::morph(void (*function)(VirtualVector vector))
{
    for (int i=0; i<vertexArray.getNVertices(); i++)
    {
        function(vertexArray.position(i));
    }
}

int Mesh::getNVertices()
{
    return vertexArray.getNVertices();
}

float *Mesh::getVertices()
{
    return vertexArray.getArrayPointer();
}

int Mesh::getNIndices()
{
    return indexArray.getNIndices();
}

unsigned int *Mesh::getIndices()
{
    return indexArray.getArrayPointer();
}

Mesh::~Mesh()
{
    std::cout << "~Mesh" << std::endl;
}
