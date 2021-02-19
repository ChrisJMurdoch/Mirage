
#include "model/virtualVector.h"

#include <math.h>

VirtualVector::VirtualVector(float *ptr) : ptr(ptr)
{

}

void VirtualVector::set(float x, float y, float z)
{
	ptr[0] = x;
	ptr[1] = y;
	ptr[2] = z;
}

void VirtualVector::setX(float value)
{
	ptr[0] = value;
}

void VirtualVector::setY(float value)
{
	ptr[1] = value;
}

void VirtualVector::setZ(float value)
{
	ptr[2] = value;
}

float VirtualVector::getX()
{
	return ptr[0];
}

float VirtualVector::getY()
{
	return ptr[1];
}

float VirtualVector::getZ()
{
	return ptr[2];
}

void VirtualVector::normalise()
{
	float magnitude = sqrt( pow(ptr[0],2) + pow(ptr[1],2) + pow(ptr[2],2) );
	set( getX()/magnitude, getY()/magnitude, getZ()/magnitude );
}
