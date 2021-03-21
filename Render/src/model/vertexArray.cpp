
#include "model/vertexArray.hpp"

#include "model/model.hpp"

#include <iostream>

VertexArray::VertexArray(int nVertices) : nVertices(nVertices)
{
	vertices = new float[(long long)nVertices*Model::STRIDE];
	std::cout << "+VertexArray" << std::endl;
}

// Vertex access
int VertexArray::getNVertices() const
{
	return nVertices;
}
VirtualVector VertexArray::position(int i)
{
	return VirtualVector(&vertices[(i*Model::STRIDE)+Model::POSITION_INDEX]);
}
VirtualVector VertexArray::normal(int i)
{
	return VirtualVector(&vertices[(i*Model::STRIDE)+Model::NORMAL_INDEX]);
}
VirtualVector VertexArray::colour(int i)
{
	return VirtualVector(&vertices[(i*Model::STRIDE)+Model::COLOUR_INDEX]);
}
VirtualVector const VertexArray::position(int i) const
{
	return VirtualVector(&vertices[(i*Model::STRIDE)+Model::POSITION_INDEX]);
}
VirtualVector const VertexArray::normal(int i) const
{
	return VirtualVector(&vertices[(i*Model::STRIDE)+Model::NORMAL_INDEX]);
}
VirtualVector const VertexArray::colour(int i) const
{
	return VirtualVector(&vertices[(i*Model::STRIDE)+Model::COLOUR_INDEX]);
}

// Raw access
int VertexArray::getArrayLength() const
{
	return nVertices *Model::STRIDE;
}
float *VertexArray::getArrayPointer()
{
	return vertices;
}
float const *VertexArray::getArrayPointer() const
{
	return vertices;
}

VertexArray::~VertexArray()
{
	delete[] vertices;
	std::cout << "~VertexArray" << std::endl;
}
