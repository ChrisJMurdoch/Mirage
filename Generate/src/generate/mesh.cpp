
#include "generate\mesh.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <exception>

const int CUBE_FACES = 6, TRIANGLES_PER_QUAD = 2;

Mesh::Mesh(int edgeVertices, AttributeSpecifier *spec) : indexArray( (edgeVertices-1) * (edgeVertices-1) * TRIANGLES_PER_QUAD * CUBE_FACES)
{
    if (edgeVertices < 2)
        throw std::exception("Mesh cannot have < 2 edge vertices.");
    int edgeLines = edgeVertices-1;

    // Save attribute specifier
    this->spec = spec;
    
    // Create data arrays
    nVertices = edgeVertices*edgeVertices*CUBE_FACES;
    vertices = new float[(long long)nVertices*spec->STRIDE];

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
    
    std::cout << "+Mesh - nV=" << nVertices << " nI=" << indexArray.getNIndices() << std::endl;
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
            int i = (((a*ev) + b + verticesOffset) * spec->STRIDE);

            // Set position
            if (spec->hasAttribute(spec->POSITION_INDEX))
            {
                vertices[i+spec->POSITION_INDEX+0] = origin.x + a*delta.x;
                vertices[i+spec->POSITION_INDEX+1] = origin.y + b*delta.y;
                if (delta.z == 0)
                    vertices[i+spec->POSITION_INDEX+2] = origin.z;
                else
                    vertices[i+spec->POSITION_INDEX+2] = (delta.x==0) ? (origin.z + a*delta.z) : (origin.z + b*delta.z);
            }

            // Set normal
            if (spec->hasAttribute(spec->NORMAL_INDEX))
            {

            }

            // Set colour
            if (spec->hasAttribute(spec->COLOUR_INDEX))
            {
                vertices[i+spec->COLOUR_INDEX+0] = float(rand()) / float((RAND_MAX));
                vertices[i+spec->COLOUR_INDEX+1] = float(rand()) / float((RAND_MAX));
                vertices[i+spec->COLOUR_INDEX+2] = float(rand()) / float((RAND_MAX));
            }
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

void Mesh::morph(void (*function)(glm::vec3 &vector))
{
    // For every vertex
    for (int i=0; i<nVertices; i++)
    {
        // Get index of vertex
        int index = i * spec->STRIDE;

        // Parse to vec3
        glm::vec3 vector = glm::vec3(
            vertices[index+spec->POSITION_INDEX+0],
            vertices[index+spec->POSITION_INDEX+1],
            vertices[index+spec->POSITION_INDEX+2]
        );

        // Perform morph
        function(vector);

        // Return to array
        vertices[index+spec->POSITION_INDEX+0] = vector.x;
        vertices[index+spec->POSITION_INDEX+1] = vector.y;
        vertices[index+spec->POSITION_INDEX+2] = vector.z;
    }
}

int Mesh::getNVertices()
{
    return nVertices;
}

float *Mesh::getVertices()
{
    return vertices;
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
    delete[] vertices;
    std::cout << "~Mesh" << std::endl;
}
