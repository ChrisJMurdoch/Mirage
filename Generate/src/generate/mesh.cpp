
#include "generate\mesh.h"

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
    nVertices = ev*ev, nIndices = (el*el) * 6;
    vertices = new float[(long long)nVertices*vertStride];
    indices = new unsigned int[(long long)nIndices];

    // Space between vertices
    float space = 1.0f / el;

    // Generate vertices
    for (int y=0; y<ev; y++)
    {
        for (int x=0; x<ev; x++)
        {
            int i = ( (y*ev) + x ) * vertStride;
            
            vertices[i+vertCoord+0] = x*space - 0.5f;
            vertices[i+vertCoord+1] = y*space - 0.5f;
            vertices[i+vertCoord+2] = 0;

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
            int i = ( (y*el) + x ) * 6;

            int xo=x, yo=y, xp=x+1, yp=y+1;
            int a, b, c, d;
            if (toggle = !toggle)
                a = (yo*ev)+xo, b = (yo*ev)+xp, c = (yp*ev)+xp, d = (yp*ev)+xo;
            else
                a = (yo*ev)+xp, b = (yp*ev)+xp, c = (yp*ev)+xo, d = (yo*ev)+xo;

            indices[i+0] = a;
            indices[i+1] = b;
            indices[i+2] = c;
            indices[i+3] = c;
            indices[i+4] = d;
            indices[i+5] = a;
        }
    }

    std::cout << "+Mesh - nV=" << nVertices << " nI=" << nIndices << std::endl;
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
