
#pragma once

#include <glm/glm.hpp>

#include <list>

class Program
{
public:
	enum class Shader { VERTEX, FRAGMENT };
private:
	unsigned int id;
	std::list<unsigned int> shaders;
public:
	Program();
	void addShader(const char *source, Shader type);
	void link();
	void setUniform4f(const char *name, float x, float y, float z, float w);
	void setUniformMatrix4fv(const char *name, glm::mat4 &matrix);
	void use();
	~Program();
};
