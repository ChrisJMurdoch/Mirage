
#include "generate\mesh.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <exception>

Mesh::Mesh(int edgeVertices, AttributeSpecifier *spec)
{
    if (edgeVertices < 2)
        throw std::exception("Mesh cannot have edge vertices < 2.");

    this->spec = spec;

    // Shorthand edge-vertices and edge-lines
    int ev = edgeVertices, el = edgeVertices-1;

    // Array data
    static const int FACES = 6;
    nVertices = ev*ev*FACES, nIndices = (el*el)*6*FACES;
    vertices = new float[(long long)nVertices*spec->STRIDE];
    indices = new unsigned int[(long long)nIndices];

    // Setup
    int faceVertStride = nVertices/FACES;
    int faceIndiStride = nIndices/FACES;
    float space = 1.0f / el;
    Coord o, d;

    // Faces
    o.x=-0.5f, o.y=-0.5f, o.z=-0.5f, d.x=space, d.y=space, d.z=0;
    generatePlane(edgeVertices, faceVertStride*0, faceIndiStride*0, o, d);
    o.x=-0.5f, o.y=-0.5f, o.z=0.5f, d.x=space, d.y=space, d.z=0;
    generatePlane(edgeVertices, faceVertStride*1, faceIndiStride*1, o, d);
    o.x=-0.5f, o.y=0.5f, o.z=-0.5f, d.x=space, d.y=0, d.z=space;
    generatePlane(edgeVertices, faceVertStride*2, faceIndiStride*2, o, d);
    o.x=-0.5f, o.y=-0.5f, o.z=-0.5f, d.x=space, d.y=0, d.z=space;
    generatePlane(edgeVertices, faceVertStride*3, faceIndiStride*3, o, d);
    o.x=0.5f, o.y=-0.5f, o.z=-0.5f, d.x=0, d.y=space, d.z=space;
    generatePlane(edgeVertices, faceVertStride*4, faceIndiStride*4, o, d);
    o.x=-0.5f, o.y=-0.5f, o.z=-0.5f, d.x=0, d.y=space, d.z=space;
    generatePlane(edgeVertices, faceVertStride*5, faceIndiStride*5, o, d);
    
    std::cout << "+Mesh - nV=" << nVertices << " nI=" << nIndices << std::endl;
}

void Mesh::generatePlane(int edgeVertices, int verticesOffset, int indicesOffset, Coord origin, Coord delta)
{
    // Shorthand edge-vertices and edge-lines
    int ev = edgeVertices, el = edgeVertices-1;

    // Generate vertices
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

    // Alternate quad splitting
    bool toggle = false;

    // Generate indices
    for (int y=0; y<el; y++)
    {
        for (int x=0; x<el; x++)
        {
            // Calculate index for indices
            int i = (((y*el) + x) * 6) + indicesOffset;

            // Calulate indices for vertices
            int xo=x, yo=y, xp=x+1, yp=y+1;
            int topleft = (yo*ev)+xo,
                topright = (yo*ev)+xp,
                botright = (yp*ev)+xp,
                botleft = (yp*ev)+xo;

            // Triangle 1
            indices[i+0] = verticesOffset+topleft;
            indices[i+1] = verticesOffset+topright;
            indices[i+2] = verticesOffset+botright;

            // Triangle 2
            indices[i+3] = verticesOffset+botright;
            indices[i+4] = verticesOffset+botleft;
            indices[i+5] = verticesOffset+topleft;
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
    return nIndices;
}

unsigned int *Mesh::getIndices()
{
    return indices;
}

Mesh::~Mesh()
{
    delete[] vertices;
    delete[] indices;
    std::cout << "~Mesh" << std::endl;
}
