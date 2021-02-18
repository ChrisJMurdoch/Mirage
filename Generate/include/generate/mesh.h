
#pragma once

class Mesh
{
private:
	int nVertices, nIndices;
	float *vertices;
	unsigned int *indices;
public:
	Mesh(int dimension);
	int getNVertices();
	float *getVertices();
	int getNIndices();
	unsigned int *getIndices();
	~Mesh();
};
