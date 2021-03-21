
#include "model/indexArray.hpp"

#include <iostream>

IndexArray::IndexArray(int nTriangles) : nIndices(nTriangles*3)
{
	indices = new unsigned int[nIndices];
	std::cout << "+IndexArray" << std::endl;
}

// Triangle access
int IndexArray::getNTriangles() const
{
	return nIndices/3;
}
unsigned int *IndexArray::operator[](int i)
{
	return &indices[i*3];
}
unsigned int const *IndexArray::operator[](int i) const
{
	return &indices[i*3];
}

// Index access
int IndexArray::getNIndices() const
{
	return nIndices;
}
unsigned int *IndexArray::getArrayPointer()
{
	return indices;
}
unsigned int const *IndexArray::getArrayPointer() const
{
	return indices;
}

IndexArray::~IndexArray()
{
	delete[] indices;
	std::cout << "~IndexArray" << std::endl;
}
