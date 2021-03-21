
#pragma once

class IndexArray
{
private:
	const int nIndices;
	unsigned int *indices;
public:
	IndexArray(int nTriangles);

	// Triangle access
	int getNTriangles() const;
	unsigned int *operator[](int i);
	unsigned int const *operator[](int i) const;

	// Index access
	int getNIndices() const;
	unsigned int *getArrayPointer();
	unsigned int const *getArrayPointer() const;

	~IndexArray();
};
