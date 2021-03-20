
#include "model/indexArray.hpp"

#include <iostream>

IndexArray::IndexArray(int nTriangles) : nIndices(nTriangles*3)
{
	indices = new unsigned int[nIndices];
	std::cout << "+IndexArray" << std::endl;
}

// Triangle access
int IndexArray::getNTriangles()
{
	return nIndices/3;
}
unsigned int *IndexArray::operator[](int i)
{
	return &indices[i*3];
}

// Index access
int IndexArray::getNIndices()
{
	return nIndices;
}
unsigned int *IndexArray::getArrayPointer()
{
	return indices;
}

IndexArray::~IndexArray()
{
	delete[] indices;
	std::cout << "~IndexArray" << std::endl;
}
