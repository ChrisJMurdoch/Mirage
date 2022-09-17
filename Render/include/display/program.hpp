
#pragma once

#include <functional>

class Vec3;

class Program
{
private:
    unsigned int handle;

public:
    Program(char const *vertexShaderPath, char const *fragmentShaderPath);
    ~Program();
    Program(Program const &other) = delete;
    Program &operator=(Program const &other) = delete;

    void setUniformVec3(char const *name, Vec3 const &vec3);
    void use(std::function<void()> operation) const;
};
