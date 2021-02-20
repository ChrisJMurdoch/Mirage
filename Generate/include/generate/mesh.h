
#pragma once

#include "model/vertexArray.h"
#include "model/indexArray.h"

#include <unordered_map>

namespace mesh
{
	struct Coord { float x, y, z; };
	int vertexCount(int edgeVertices);
	int triangleCount(int edgeVertices);
	void generateCube(int edgeVertices, VertexArray &vertexArray, IndexArray &indexArray);
	void planet(VertexArray &vertexArray, int octaves);
	void fixNormals(VertexArray &vertexArray, IndexArray &indexArray);
};
