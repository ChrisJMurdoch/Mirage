
#include "hexEngine/hexCell.hpp"

#include <glm/glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <unordered_map>

float const HexCell::APOTHEM = 0.5f, HexCell::RADIUS = APOTHEM * 2.0f / sqrt(3.0f);

HexCell::HexCell() : vertices(30), indices(16)
{
	// X & Z coordinates starting from top-left, winding clockwise
	float const x[6] = {
		-RADIUS/2.0f, RADIUS/2.0f, RADIUS,
		RADIUS/2.0f, -RADIUS/2.0f, -RADIUS
	};
	float const z[6] = {
		APOTHEM, APOTHEM, 0.0f,
		-APOTHEM, -APOTHEM, 0.0f,
	};

	// Top
	for (int i=0; i<6; i++)
		vertices.position(i)->xyz( x[i], 0.5f, z[i] );
	indices.tri(0)->xyz(2, 1, 0);
	indices.tri(1)->xyz(4, 3, 2);
	indices.tri(2)->xyz(0, 5, 4);
	indices.tri(3)->xyz(4, 2, 0);

	// Sides
	for (int i=0; i<6; i++)
	{
		int v_i = 6+(i*4);
		int i_i = 4+(i*2);
		vertices.position(v_i+0)->xyz(x[(i+0)%6], 1.5f, z[(i+0)%6]);
		vertices.position(v_i+1)->xyz(x[(i+1)%6], 1.5f, z[(i+1)%6]);
		vertices.position(v_i+2)->xyz(x[(i+1)%6], 0.5f, z[(i+1)%6]);
		vertices.position(v_i+3)->xyz(x[(i+0)%6], 0.5f, z[(i+0)%6]);
		indices.tri(i_i+0)->xyz(v_i+2, v_i+1, v_i+0);
		indices.tri(i_i+1)->xyz(v_i+0, v_i+3, v_i+2);
	}

	// Colour
	for (int i=0; i<vertices.getElementLength(); i++)
	{
		vertices.colour(i)->xyz(0.3f, 0.8f, 0.1f);
	}

	// Normals
	fixNormals();
}

PNC const &HexCell::getVertices() const
{
	return vertices;
}

Tri const &HexCell::getIndices() const
{
	return indices;
}

/* Calculate normals for each vertex by averaging normals of composite triangles */
void HexCell::fixNormals()
{
	std::unordered_map<int, int> occurrences;
	std::unordered_map<int, glm::vec3> normals;

	// Every triangle
	for (int i=0; i<indices.getElementLength(); i++)
	{
		// Get tri
		Vec3<unsigned int> *tri = indices.tri(i);
		glm::vec3
			a = vertices.position(tri->x())->glm(),
			b = vertices.position(tri->y())->glm(),
			c = vertices.position(tri->z())->glm();

		// Calculate normal of tri
		glm::vec3 normal = glm::cross((c-b), (a-b));

		// Increment occurrences
		occurrences[tri->x()]++;
		occurrences[tri->y()]++;
		occurrences[tri->z()]++;

		// Add normal to totals
		normals[tri->x()] += normal;
		normals[tri->y()] += normal;
		normals[tri->z()] += normal;
	}

	// Set vertices
	for (int i=0; i<vertices.getElementLength(); i++)
	{
		glm::vec3 normal = glm::normalize(normals[i] / (float)occurrences[i]);
		vertices.normal(i)->xyz(normal.x, normal.y, normal.z);
	}
}
