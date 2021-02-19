
#include "model/vertexArray.h"

#include <iostream>

VertexArray::VertexArray(int nVertices, AttributeSpecifier *spec) : nVertices(nVertices), spec(spec)
{
	vertices = new float[(long long)nVertices*spec->STRIDE];
	std::cout << "+VertexArray" << std::endl;
}

// Vertex access
int VertexArray::getNVertices()
{
	return nVertices;
}
VirtualVector VertexArray::position(int i)
{
	return VirtualVector(&vertices[(i*spec->STRIDE)+spec->POSITION_INDEX]);
}
VirtualVector VertexArray::normal(int i)
{
	return VirtualVector(&vertices[(i*spec->STRIDE)+spec->NORMAL_INDEX]);
}
VirtualVector VertexArray::colour(int i)
{
	return VirtualVector(&vertices[(i*spec->STRIDE)+spec->COLOUR_INDEX]);
}

// Raw access
int VertexArray::getArrayLength()
{
	return nVertices *spec->STRIDE;
}
float *VertexArray::getArrayPointer()
{
	return vertices;
}

VertexArray::~VertexArray()
{
	delete[] vertices;
	std::cout << "~VertexArray" << std::endl;
}
