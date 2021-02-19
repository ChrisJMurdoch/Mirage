
#pragma once

#include "glm/glm.hpp"

class VirtualVector
{
private:
	float *const ptr;
public:
	VirtualVector(float *ptr);

	// Setters
	void set(float x, float y, float z);
	void setX(float value);
	void setY(float value);
	void setZ(float value);

	// Getters
	float getX();
	float getY();
	float getZ();

	// Manipulators
	void normalise();
	void multiply(float value);

	// Export
	glm::vec3 asVec3();
};
