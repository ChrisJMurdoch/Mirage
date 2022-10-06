
#pragma once

#include <vector>

class Program;
struct Material;
struct Vertex;

class Model
{
private:
    Program const &program;
    Material const &material;
    unsigned int vao, vbo, ebo;
    int const nIndices;

public:
    Model(Program const &program, std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices, Material const &material);
    Model(Model &&other);
    ~Model();
    Model(Model const &other) = delete;
    Model &operator=(Model const &other) = delete;

    void draw() const;
};
