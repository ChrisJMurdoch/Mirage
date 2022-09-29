
#pragma once

#include <glm/fwd.hpp>

#include <functional>

class Program
{
private:
    unsigned int handle;

public:
    Program(char const *vertexShaderPath, char const *fragmentShaderPath);
    ~Program();
    Program(Program const &other) = delete;
    Program &operator=(Program const &other) = delete;

    void setUniformVec3(char const *name, glm::vec3 const &vec);
    void setUniformMat4(char const *name, glm::mat4 const &mat);
    void use(std::function<void()> operation) const;
};
