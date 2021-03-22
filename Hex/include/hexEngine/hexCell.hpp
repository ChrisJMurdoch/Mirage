
#pragma once

#include "model/vertexArray.hpp"
#include "model/indexArray.hpp"

class HexCell
{
private:
	VertexArray vertices;
	IndexArray indices;

public:
	HexCell();

	VertexArray const &getVertices() const;
	IndexArray const &getIndices() const;
};
