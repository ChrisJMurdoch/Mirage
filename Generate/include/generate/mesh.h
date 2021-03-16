
#pragma once

#include "model/vertexArray.h"
#include "model/indexArray.h"
#include "model/virtualVector.h"

#include <unordered_map>

namespace mesh
{
	class Shape
	{
	public:
		virtual void setPosition(VirtualVector position) = 0;
		virtual void setColour(VirtualVector position, VirtualVector normal, VirtualVector colour) = 0;
	};
	struct Coord { float x, y, z; };
	int vertexCount(int edgeVertices);
	int triangleCount(int edgeVertices);
	void generateCube(int edgeVertices, VertexArray &vertexArray, IndexArray &indexArray);
	void setPositions(VertexArray &vertexArray, Shape *shape, int threads=1);
	void setColours(VertexArray &vertexArray, Shape *shape, int threads=1);
	void fixNormals(VertexArray &vertexArray, IndexArray &indexArray);
};
