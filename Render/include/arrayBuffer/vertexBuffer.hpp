
#pragma once

#include <glad/glad.h>

#include <string>

/* Simple 3-part vector */
template<typename T>
class Vec3
{
private:
	T xv, yv, zv;
public:
	T &x() { return xv; }
	T &y() { return yv; }
	T &z() { return zv; }
	T const &x() const { return xv; }
	T const &y() const { return yv; }
	T const &z() const { return zv; }
	void x(T x) { this->xv = x; }
	void y(T y) { this->yv = y; }
	void z(T z) { this->zv = z; }
	void xy(T x, T y) { this->xv = x; this->yv = y; }
	void yz(T y, T z) { this->yv = y; this->zv = z; }
	void xz(T x, T z) { this->xv = x; this->zv = z; }
	void xyz(T x, T y, T z) { this->xv = x; this->yv = y; this->zv = z; }
	std::string s() const { return "["+std::to_string(xv)+"|"+std::to_string(yv)+"|"+std::to_string(zv)+"]"; }
};

/* Abstract vertex buffer object */
template <typename  T>
class VertexBuffer
{
protected:
	unsigned int id;
	int const length;
	T * const data;

protected:
	VertexBuffer(int length) : length(length), data(new T[length])
	{
		glGenBuffers(1, &id);
	}

	/* Bind relevant VAO and VBO before calling. */
	void setAttribute(int index, int size, int stride, int ptr) const
	{
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void *)(ptr*sizeof(float)));
		glEnableVertexAttribArray(index);
	}

public:
	/* Bind relevant VAO before calling. */
	virtual void bufferData() const = 0;

	int getLength() const
	{
		return length;
	}

	~VertexBuffer()
	{
		delete[] data;
	}
};

/* 9-float VertexBuffer implementation for: position, normal, colour */
class PNC : public VertexBuffer<float>
{
private:
	static int const POS_PTR = 0, NOR_PTR = 3, COL_PTR = 6, STRIDE = 9;
public:
	PNC(int length);
	/* Bind relevant VAO before calling. */
	virtual void bufferData() const;

	Vec3<float> *position(int index);
	Vec3<float> *normal(int index);
	Vec3<float> *colour(int index);
	Vec3<float> const *position(int index) const;
	Vec3<float> const *normal(int index) const;
	Vec3<float> const *colour(int index) const;
};

/* 3-int VertexBuffer implementation for: index, index, index */
class Tri : public VertexBuffer<unsigned int>
{
private:
	static int const TRI_PTR = 0, STRIDE = 3;
public:
	Tri(int length);
	/* Bind relevant VAO before calling. */
	virtual void bufferData() const;

	Vec3<unsigned int> *tri(int index);
	Vec3<unsigned int> const *tri(int index) const;
};
