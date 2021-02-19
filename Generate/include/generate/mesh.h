
#pragma once

#include "model/vertexArray.h"
#include "model/indexArray.h"

namespace mesh
{
	struct Coord { float x, y, z; };
	int vertexCount(int edgeVertices);
	int indexCount(int edgeVertices);
	void generateCube(int edgeVertices, VertexArray *vertexArray, IndexArray *indexArray);
	void morph(VertexArray *vertexArray, void (*function)(VirtualVector vector));
};
