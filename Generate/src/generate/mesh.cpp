
#include "generate\mesh.h"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <exception>

Mesh::Mesh(int edgeVertices, int vertStride, int vertCoord, int vertColour)
{
    if (edgeVertices < 2)
        throw std::exception("Mesh cannot have edge vertices < 2.");

    // Shorthand edge-vertices and edge-lines
    int ev = edgeVertices, el = edgeVertices-1;

    // Array data
    static const int FACES = 6;
    nVertices = ev*ev*FACES, nIndices = (el*el)*6*FACES;
    vertices = new float[(long long)nVertices*vertStride];
    indices = new unsigned int[(long long)nIndices];

    // Setup
    int faceVertStride = nVertices/FACES;
    int faceIndiStride = nIndices/FACES;
    float space = 1.0f / el;
    Coord o, d;

    // Faces
    o.x=-0.5f, o.y=-0.5f, o.z=-0.5f, d.x=space, d.y=space, d.z=0;
    generatePlane(edgeVertices, vertStride, vertCoord, vertColour, faceVertStride*0, faceIndiStride*0, o, d);
    o.x=-0.5f, o.y=-0.5f, o.z=0.5f, d.x=space, d.y=space, d.z=0;
    generatePlane(edgeVertices, vertStride, vertCoord, vertColour, faceVertStride*1, faceIndiStride*1, o, d);
    o.x=-0.5f, o.y=0.5f, o.z=-0.5f, d.x=space, d.y=0, d.z=space;
    generatePlane(edgeVertices, vertStride, vertCoord, vertColour, faceVertStride*2, faceIndiStride*2, o, d);
    o.x=-0.5f, o.y=-0.5f, o.z=-0.5f, d.x=space, d.y=0, d.z=space;
    generatePlane(edgeVertices, vertStride, vertCoord, vertColour, faceVertStride*3, faceIndiStride*3, o, d);
    o.x=0.5f, o.y=-0.5f, o.z=-0.5f, d.x=0, d.y=space, d.z=space;
    generatePlane(edgeVertices, vertStride, vertCoord, vertColour, faceVertStride*4, faceIndiStride*4, o, d);
    o.x=-0.5f, o.y=-0.5f, o.z=-0.5f, d.x=0, d.y=space, d.z=space;
    generatePlane(edgeVertices, vertStride, vertCoord, vertColour, faceVertStride*5, faceIndiStride*5, o, d);
    
    sphere(nVertices, vertices, vertStride, vertCoord);

    std::cout << "+Mesh - nV=" << nVertices << " nI=" << nIndices << std::endl;
}

void Mesh::generatePlane(int edgeVertices, int vertStride, int vertCoord, int vertColour, int vertOffset, int indiOffset, Coord o, Coord d)
{
    // Shorthand edge-vertices and edge-lines
    int ev = edgeVertices, el = edgeVertices-1;

    // Generate vertices
    for (int a=0; a<ev; a++)
    {
        for (int b=0; b<ev; b++)
        {
            int i = (((a*ev) + b + vertOffset) * vertStride);

            vertices[i+vertCoord+0] = o.x + a*d.x;
            vertices[i+vertCoord+1] = o.y + b*d.y;
            if (d.z == 0)
                vertices[i+vertCoord+2] = o.z;
            else
                vertices[i+vertCoord+2] = (d.x==0) ? (o.z + a*d.z) : (o.z + b*d.z);

            vertices[i+vertColour+0] = float(rand()) / float((RAND_MAX));
            vertices[i+vertColour+1] = float(rand()) / float((RAND_MAX));
            vertices[i+vertColour+2] = float(rand()) / float((RAND_MAX));

        }
    }

    // Generate indices
    bool toggle = false;
    for (int y=0; y<el; y++)
    {
        for (int x=0; x<el; x++)
        {
            int i = (((y*el) + x) * 6) + indiOffset;

            int xo=x, yo=y, xp=x+1, yp=y+1;
            int a, b, c, d;
            if (toggle = !toggle)
                a = (yo*ev)+xo, b = (yo*ev)+xp, c = (yp*ev)+xp, d = (yp*ev)+xo;
            else
                a = (yo*ev)+xp, b = (yp*ev)+xp, c = (yp*ev)+xo, d = (yo*ev)+xo;

            indices[i+0] = a+vertOffset;
            indices[i+1] = b+vertOffset;
            indices[i+2] = c+vertOffset;
            indices[i+3] = c+vertOffset;
            indices[i+4] = d+vertOffset;
            indices[i+5] = a+vertOffset;
        }
    }
}

void Mesh::sphere(int nVertices, float *vertices, int vertStride, int vertCoord)
{
    for (int i=0; i<nVertices; i++)
    {
        int index = i * vertStride;

        // Parse to vec3 and normalise
        glm::vec3 vector = glm::vec3( vertices[index+vertCoord+0], vertices[index+vertCoord+1], vertices[index+vertCoord+2] );
        vector = glm::normalize(vector);

        // Return to array
        vertices[index+vertCoord+0] = vector.x;
        vertices[index+vertCoord+1] = vector.y;
        vertices[index+vertCoord+2] = vector.z;
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
