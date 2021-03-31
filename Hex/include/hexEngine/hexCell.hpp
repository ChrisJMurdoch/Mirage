
#pragma once

#include "arrayBuffer/vertexBuffer.hpp"

class HexCell
{
private:
	PNC vertices;
	Tri indices;

public:
	HexCell();

	PNC const &getVertices() const;
	Tri const &getIndices() const;
};
