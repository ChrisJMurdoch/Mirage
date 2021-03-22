
#include "hexEngine/hexCell.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

HexCell::HexCell() : vertices(12), indices(16)
{
	static float const APOTHEM = 0.5f, RADIUS = (2.0f * APOTHEM) / sqrt(3.0f), HEIGHT = 0.25f;
	
	// Top
	vertices.position(0).set( -RADIUS/2.0f, HEIGHT,  APOTHEM );
	vertices.position(1).set(  RADIUS/2.0f, HEIGHT,  APOTHEM );
	vertices.position(2).set(  RADIUS,      HEIGHT,  0.0f    );
	vertices.position(3).set(  RADIUS/2.0f, HEIGHT, -APOTHEM );
	vertices.position(4).set( -RADIUS/2.0f, HEIGHT, -APOTHEM );
	vertices.position(5).set( -RADIUS,      HEIGHT,  0.0f    );

	// Bottom
	vertices.position(6 ).set( -RADIUS/2.0f, HEIGHT+1.0f, APOTHEM  );
	vertices.position(7 ).set(  RADIUS/2.0f, HEIGHT+1.0f, APOTHEM  );
	vertices.position(8 ).set(  RADIUS,      HEIGHT+1.0f, 0.0f     );
	vertices.position(9 ).set(  RADIUS/2.0f, HEIGHT+1.0f, -APOTHEM );
	vertices.position(10).set( -RADIUS/2.0f, HEIGHT+1.0f, -APOTHEM );
	vertices.position(11).set( -RADIUS,      HEIGHT+1.0f, 0.0f     );

	// Top
	indices[0][0] = 2;
	indices[0][1] = 1;
	indices[0][2] = 0;
	indices[1][0] = 4;
	indices[1][1] = 3;
	indices[1][2] = 2;
	indices[2][0] = 0;
	indices[2][1] = 5;
	indices[2][2] = 4;
	indices[3][0] = 4;
	indices[3][1] = 2;
	indices[3][2] = 0;

	// Sides
	for (int i=0; i<6; i++)
	{
		int const tri = (i*2) + 4;
		indices[tri+0][0] = ((i+0)%6);
		indices[tri+0][1] = ((i+7)%6)+6;
		indices[tri+0][2] = ((i+6)%6)+6;
		indices[tri+1][0] = ((i+0)%6);
		indices[tri+1][1] = ((i+1)%6);
		indices[tri+1][2] = ((i+7)%6)+6;
	}

	// Sides
	for (int i=0; i<12; i++)
	{
		vertices.colour(i).set(i%2==0, i%3==0, i%5==0);
		vertices.normal(i).set(0.0f, -1.0f, 0.0f);
	}
}

VertexArray const &HexCell::getVertices() const
{
	return vertices;
}

IndexArray const &HexCell::getIndices() const
{
	return indices;
}
