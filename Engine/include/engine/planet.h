
#pragma once

#include "model/virtualVector.h"
#include "generate/mesh.h"

class Planet: public mesh::Shape
{
private:
	int const octaves;
public:
	Planet(int seed);
	virtual void setPosition(VirtualVector position);
	virtual void setColour(VirtualVector position, VirtualVector normal, VirtualVector colour);
};
