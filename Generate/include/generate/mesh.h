
#pragma once

#include "render/attributeSpecifier.h"

#include <glm/glm.hpp>

class Mesh
{
private:
	struct Coord { float x, y, z; };
	AttributeSpecifier *spec;
	int nVertices, nIndices;
	float *vertices;
	unsigned int *indices;
public:
	Mesh(int edgeVertices, AttributeSpecifier *spec);
	void generatePlane(int edgeVertices, int vertOffset, int indiOffset, Coord o, Coord d);
	void morph(void (*function)(glm::vec3 &vector));
	int getNVertices();
	float *getVertices();
	int getNIndices();
	unsigned int *getIndices();
	~Mesh();
};
