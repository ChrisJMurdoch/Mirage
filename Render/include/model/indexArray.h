
#pragma once

class IndexArray
{
private:
	const int nIndices;
	unsigned int *indices;
public:
	IndexArray(int nIndices, unsigned int *indices);
	int getNIndices();
	unsigned int *getArrayPointer();
	unsigned int *operator[](int i);
	~IndexArray();
};
