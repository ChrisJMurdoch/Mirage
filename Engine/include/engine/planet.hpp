
#pragma once

#include "model/virtualVector.hpp"
#include "generate/mesh.hpp"

class Planet: public mesh::Shape
{
private:
	int const octaves;
public:
	Planet(int seed);
	virtual void setPosition(VirtualVector position);
	virtual void setColour(VirtualVector position, VirtualVector normal, VirtualVector colour);
};
