
#pragma once

class Mesh
{
private:
	int nVertices, nIndices;
	float *vertices;
	unsigned int *indices;
public:
	Mesh(int edgeVertices, int vertStride, int vertCoord, int vertColour);
	int getNVertices();
	float *getVertices();
	int getNIndices();
	unsigned int *getIndices();
	~Mesh();
};
