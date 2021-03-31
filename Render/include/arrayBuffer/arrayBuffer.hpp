
#pragma once

#include <glm/glm.hpp>
#include <string>

/* Simple 3-float vector */
class Vec3F
{
private:
	float xv, yv, zv;
public:
	float &x() { return xv; }
	float &y() { return yv; }
	float &z() { return zv; }
	void x(float x) { this->xv = x; }
	void y(float y) { this->yv = y; }
	void z(float z) { this->zv = z; }
	void xy(float x, float y) { this->xv = x; this->yv = y; }
	void yz(float y, float z) { this->yv = y; this->zv = z; }
	void xz(float x, float z) { this->xv = x; this->zv = z; }
	void xyz(float x, float y, float z) { this->xv = x; this->yv = y; this->zv = z; }
	std::string s() const { return "["+std::to_string(xv)+"|"+std::to_string(yv)+"|"+std::to_string(zv)+"]"; }
};

/* Abstract array buffer */
class ArrayBuffer
{
protected:
	unsigned int id;
	float * const data;
	int const size;
public:
	ArrayBuffer(int size);
	virtual void bufferData(unsigned int vao) = 0;
	void setAttribute(int index, int size, int stride, int ptr);
};

/* ArrayBuffer implementation: Position, Normal, Colour */
class PNC : public ArrayBuffer
{
private:
	static int const POS_PTR = 0, NOR_PTR = 3, COL_PTR = 6, STRIDE = 9;
public:
	PNC(int size);
	virtual void bufferData(unsigned int vao);
	Vec3F *position(int index);
	Vec3F *normal(int index);
	Vec3F *colour(int index);
};
