
#pragma once

/*
	3-value vector that sits on top of a float array.
	x, y, and z are aliases for indices
*/
class VirtualVector
{
private:
	float *const ptr;
public:
	VirtualVector(float *ptr);
	void set(float x, float y, float z);
	void setX(float value);
	void setY(float value);
	void setZ(float value);
	float getX();
	float getY();
	float getZ();
	void normalise();
};
