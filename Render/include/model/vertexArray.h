
#pragma once

#include "render\attributeSpecifier.h"
#include "model\virtualVector.h"

#include "glm\glm.hpp"

class VertexArray
{
private:
	const int nVertices;
	float *vertices;
	AttributeSpecifier *const spec;
public:
	VertexArray(int nVertices, AttributeSpecifier *spec);

	// Vertex access
	int getNVertices();
	VirtualVector position(int i);
	VirtualVector normal(int i);
	VirtualVector colour(int i);

	// Raw access
	int getArrayLength();
	float *getArrayPointer();

	~VertexArray();
private:
	void setAttribute(int i, int attr, float a, float b, float c);
};
