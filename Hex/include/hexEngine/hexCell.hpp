
#pragma once

#include "arrayBuffer/vertexBuffer.hpp"

class HexCell
{
public:
	static float const APOTHEM, RADIUS, SLOPE;

private:
	PNC vertices;
	Tri indices;

public:
	HexCell();

	PNC const &getVertices() const;
	Tri const &getIndices() const;

	void fixNormals();
};
