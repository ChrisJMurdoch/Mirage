
#pragma once

#include "model/virtualVector.h"

class VertexArray
{
private:
	const int nVertices;
	float *vertices;
public:
	VertexArray(int nVertices);

	// Vertex access
	int getNVertices();
	VirtualVector position(int i);
	VirtualVector normal(int i);
	VirtualVector colour(int i);

	// Raw access
	int getArrayLength();
	float *getArrayPointer();

	~VertexArray();
};
