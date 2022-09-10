
#pragma once

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

    void use(std::function<void()> operation) const;
};
