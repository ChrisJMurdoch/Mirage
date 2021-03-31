
#include "hexEngine/hexCell.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

HexCell::HexCell() : vertices(12), indices(16)
{
	static float const APOTHEM = 0.5f, RADIUS = (2.0f * APOTHEM) / sqrt(3.0f), HEIGHT = 0.25f;
	
	// Top
	vertices.position(0)->xyz( -RADIUS/2.0f, HEIGHT,  APOTHEM );
	vertices.position(0)->xyz( -RADIUS/2.0f, HEIGHT,  APOTHEM );
	vertices.position(1)->xyz(  RADIUS/2.0f, HEIGHT,  APOTHEM );
	vertices.position(2)->xyz(  RADIUS,      HEIGHT,  0.0f    );
	vertices.position(3)->xyz(  RADIUS/2.0f, HEIGHT, -APOTHEM );
	vertices.position(4)->xyz( -RADIUS/2.0f, HEIGHT, -APOTHEM );
	vertices.position(5)->xyz( -RADIUS,      HEIGHT,  0.0f    );

	// Bottom
	vertices.position(6 )->xyz( -RADIUS/2.0f, HEIGHT+1.0f, APOTHEM  );
	vertices.position(7 )->xyz(  RADIUS/2.0f, HEIGHT+1.0f, APOTHEM  );
	vertices.position(8 )->xyz(  RADIUS,      HEIGHT+1.0f, 0.0f     );
	vertices.position(9 )->xyz(  RADIUS/2.0f, HEIGHT+1.0f, -APOTHEM );
	vertices.position(10)->xyz( -RADIUS/2.0f, HEIGHT+1.0f, -APOTHEM );
	vertices.position(11)->xyz( -RADIUS,      HEIGHT+1.0f, 0.0f     );

	// Top
	indices.tri(0)->xyz(2, 1, 0);
	indices.tri(1)->xyz(4, 3, 2);
	indices.tri(2)->xyz(0, 5, 4);
	indices.tri(3)->xyz(4, 2, 0);

	// Sides
	for (int i=0; i<6; i++)
	{
		int const tri = (i*2) + 4;
		indices.tri(tri+0)->xyz(((i+0)%6), ((i+7)%6)+6, ((i+6)%6)+6);
		indices.tri(tri+1)->xyz(((i+0)%6), ((i+1)%6), ((i+7)%6)+6);
	}

	// Sides
	for (int i=0; i<12; i++)
	{
		vertices.colour(i)->xyz(i%2==0, i%3==0, i%5==0);
		vertices.normal(i)->xyz(0.0f, -1.0f, 0.0f);
	}
}

PNC const &HexCell::getVertices() const
{
	return vertices;
}

Tri const &HexCell::getIndices() const
{
	return indices;
}
