
#include "generate/mesh.h"

#include "model/virtualVector.h"

#include <iostream>

const int CUBE_FACES = 6, TRIANGLES_PER_QUAD = 2;

int mesh::vertexCount(int edgeVertices)
{
    return edgeVertices * edgeVertices * CUBE_FACES;
}

int mesh::indexCount(int edgeVertices)
{
    return (edgeVertices-1) * (edgeVertices-1) * TRIANGLES_PER_QUAD * CUBE_FACES;
}

void generatePlane(int edgeVertices, VertexArray *vertexArray, IndexArray *indexArray, int verticesOffset, int indicesOffset, mesh::Coord origin, mesh::Coord delta, bool wind);

void mesh::generateCube(int edgeVertices, VertexArray *vertexArray, IndexArray *indexArray)
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

void generatePlane(int edgeVertices, VertexArray *vertexArray, IndexArray *indexArray, int verticesOffset, int indicesOffset, mesh::Coord origin, mesh::Coord delta, bool wind)
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

            VirtualVector position = vertexArray->position(i);
            position.setX( origin.x + a*delta.x );
            position.setY( origin.y + b*delta.y );
            position.setZ( (delta.z == 0) ? (origin.z) : (delta.x==0) ? (origin.z + a*delta.z) : (origin.z + b*delta.z) );

            VirtualVector normal = vertexArray->normal (i);
            normal.set( position.getX(), position.getY(), position.getZ() );
            normal.normalise();

            VirtualVector colour = vertexArray->colour(i);
            colour.set( float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)) );
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

            (*indexArray)[i+0][0] = verticesOffset + topleft;
            (*indexArray)[i+0][1] = verticesOffset + a;
            (*indexArray)[i+0][2] = verticesOffset + botright;

            (*indexArray)[i+1][0] = verticesOffset + botright;
            (*indexArray)[i+1][1] = verticesOffset + b;
            (*indexArray)[i+1][2] = verticesOffset + topleft;
        }
    }
}

void mesh::morph(VertexArray *vertexArray, void (*function)(VirtualVector vector))
{
    for (int i=0; i<vertexArray->getNVertices(); i++)
    {
        function(vertexArray->position(i));
    }
}
