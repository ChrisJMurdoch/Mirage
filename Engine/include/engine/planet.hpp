
#pragma once

#include "model/vertexArray.hpp"
#include "model/indexArray.hpp"
#include "model/virtualVector.hpp"

class Planet
{
private:
	struct Coord { float x, y, z; };

private:
	int const octaves;
	VertexArray vertices;
	IndexArray indices;

	void setPositionsThread(int offset, int stride);
	void setPosition(VirtualVector position);

	void setColoursThread(int offset, int stride);
	virtual void setColour(VirtualVector position, VirtualVector normal, VirtualVector colour);

public:
	Planet(int edgevertices, int octaves);
	void generateCube(int edgeVertices);
	void generatePlane(int edgeVertices, int verticesOffset, int indicesOffset, Planet::Coord origin, Planet::Coord delta, bool wind);

	VertexArray const &getVertices() const;
	IndexArray const &getIndices() const;

	void setPositions(int threads=1);
	void fixNormals();
	void setColours(int threads=1);
};
