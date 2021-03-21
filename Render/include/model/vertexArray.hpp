
#pragma once

#include "model/virtualVector.hpp"

class VertexArray
{
private:
	const int nVertices;
	float *vertices;
public:
	VertexArray(int nVertices);

	// Vertex access
	int getNVertices() const;
	VirtualVector const position(int i) const;
	VirtualVector const normal(int i) const;
	VirtualVector const colour(int i) const;
	VirtualVector position(int i);
	VirtualVector normal(int i);
	VirtualVector colour(int i);

	// Raw access
	int getArrayLength() const;
	float *getArrayPointer();
	float const *getArrayPointer() const;

	~VertexArray();
};
