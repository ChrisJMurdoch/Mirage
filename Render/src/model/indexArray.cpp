
#include "model/indexArray.h"

IndexArray::IndexArray(int nIndices, unsigned int *indices) : nIndices(nIndices)
{
	this->indices = new unsigned int[nIndices];
}

int IndexArray::getNIndices()
{
	return nIndices;
}

unsigned int *IndexArray::getArrayPointer()
{
	return indices;
}

unsigned int *IndexArray::operator[](int i)
{
	return &indices[i*3];
}

IndexArray::~IndexArray()
{
	delete[] indices;
}
