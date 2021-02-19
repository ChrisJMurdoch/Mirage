
#pragma once

#include "render/attributeSpecifier.h"
#include "model/vertexArray.h"
#include "model/indexArray.h"

#include <glm/glm.hpp>

class Mesh
{
private:
	struct Coord { float x, y, z; };
	VertexArray vertexArray;
	IndexArray indexArray;
public:
	Mesh(int edgeVertices, AttributeSpecifier *spec);
	void generatePlane(int edgeVertices, int vertOffset, int indiOffset, Coord o, Coord d);
	void morph(void (*function)(VirtualVector vector));
	int getNVertices();
	float *getVertices();
	int getNIndices();
	unsigned int *getIndices();
	~Mesh();
};
