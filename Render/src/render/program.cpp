
#include "render\program.h"

#include <glad\glad.h>

#include <iostream>
#include <exception>

Program::Program()
{
	id = glCreateProgram();
	std::cout << "+Program" << std::endl;
}

void Program::addShader(const char* source, Shader type)
{
	// Compile
	unsigned int shader = glCreateShader( (type==Shader::VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER );
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// Debug
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
		throw std::exception("Shader-loading exception");
	}

	// Attach and track
	glAttachShader(id, shader);
	shaders.push_front(shader);

	// Display
	std::cout << " Shader compiled." << std::endl;
}

void Program::link()
{
	// Link
	glLinkProgram(id);

	// Debug
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cout << infoLog << std::endl;
		throw std::exception("Program-linking exception");
	}

	// Clean up
	for (unsigned int shader : shaders)
	{
		glDeleteShader(shader);
		std::cout << " Shader deleted." << std::endl;
	}
	shaders.clear();

	// Display
	std::cout << " Program linked." << std::endl;
}

void Program::use()
{
	glUseProgram(id);
}

Program::~Program()
{
	glDeleteProgram(id);
	std::cout << "~Program" << std::endl;
}
