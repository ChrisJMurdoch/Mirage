
#pragma once

class IndexArray
{
private:
	const int nIndices;
	unsigned int *indices;
public:
	IndexArray(int nTriangles);

	// Triangle access
	int getNTriangles();
	unsigned int *operator[](int i);

	// Index access
	int getNIndices();
	unsigned int *getArrayPointer();

	~IndexArray();
};
