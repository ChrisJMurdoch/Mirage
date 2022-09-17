
#pragma once

#include <vector>

class Program;

struct Vec3
{
    float x, y, z;
};

class Model
{
private:
    Program const &program;
    unsigned int vao, vbo, ebo;

public:
    Model(Program const &program, std::vector<Vec3> const &vertices, std::vector<unsigned int> const &indices);
    Model(Model &&other);
    ~Model();
    Model(Model const &other) = delete;
    Model &operator=(Model const &other) = delete;

    void draw() const;
};
