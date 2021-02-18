
#include "generate\mesh.h"

#include <string.h>
#include <iostream>

Mesh::Mesh(int dimension)
{
    float v[] = {
        -1, -1, 0,   1, 0, 0,
         1, -1, 0,   0, 1, 0,
         1,  1, 0,   0, 0, 1,
        -1,  1, 0,   1, 0, 1,
    };
    unsigned int i[] = {
        0, 1, 2,
        2, 3, 0,
    };
    nVertices = sizeof(v) / sizeof(v[0]);
    nIndices = sizeof(i) / sizeof(i[0]);

    vertices = new float[nVertices];
    memcpy(vertices, v, sizeof(v));
    indices = new unsigned int[nIndices];
    memcpy(indices, i, sizeof(i));

    std::cout << "+Mesh" << std::endl;
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
