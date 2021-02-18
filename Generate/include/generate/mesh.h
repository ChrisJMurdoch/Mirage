
#pragma once

class Mesh
{
private:
	struct Coord { float x, y, z; };
	int nVertices, nIndices;
	float *vertices;
	unsigned int *indices;
public:
	Mesh(int edgeVertices, int vertStride, int vertCoord, int vertColour);
	void generatePlane(int edgeVertices, int vertStride, int vertCoord, int vertColour, int vertOffset, int indiOffset, Coord o, Coord d);
	void sphere(int nVertices, float *vertices, int vertStride, int vertCoord);
	int getNVertices();
	float *getVertices();
	int getNIndices();
	unsigned int *getIndices();
	~Mesh();
};
