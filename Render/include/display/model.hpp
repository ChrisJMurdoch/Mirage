
#pragma once

#include <vector>

class Program;
class Texture;
struct Vertex;

class Model
{
private:
    Program const &program;
    Texture const &texture;
    unsigned int vao, vbo, ebo;
    int const nIndices;

public:
    Model(Program const &program, Texture const &texture, std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices);
    Model(Model &&other);
    ~Model();
    Model(Model const &other) = delete;
    Model &operator=(Model const &other) = delete;

    void draw() const;
};
