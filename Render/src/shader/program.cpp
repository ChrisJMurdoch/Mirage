
#include "shader/program.hpp"

#include <glad/glad.h>

#include <exception>
#include <fstream>
#include <sstream>

std::string readFile(char const *path){
    std::ifstream file;
    file.open(path);
    if (!file.is_open())
        throw std::exception((std::string("Could not open shader file: ") + path).c_str());
    std::stringstream sStream;
    sStream << file.rdbuf();
    return sStream.str();
}

unsigned int compileShader(char const *path, unsigned int shaderType)
{
    // Read file
    std::string source = readFile(path);
    char const *cSource = source.c_str();

    // Create and compile shader
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &cSource, nullptr);
    glCompileShader(shader);

    // Check compilation
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw std::exception(infoLog);
    }

    return shader;
}

Program::Program(char const *vertexShaderPath, char const *fragmentShaderPath)
{
    // Compile shaders
    unsigned int vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    // Link shaders into program
    handle = glCreateProgram();
    glAttachShader(handle, vertexShader);
    glAttachShader(handle, fragmentShader);
    glLinkProgram(handle);

    // Check linking
    int success;
    char infoLog[512];
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(handle, 512, NULL, infoLog);
        throw std::exception(infoLog);
    }

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  
}

Program::~Program()
{
    glDeleteProgram(handle);
}

void Program::use(std::function<void()> operation) const
{
    glUseProgram(handle);
    operation();
    glUseProgram(0);
}
