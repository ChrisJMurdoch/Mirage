
#pragma once

#include <vector>

class Program;
class Texture;
struct Vertex;

class Model
{
private:
    Program const &program;
    Texture const *const albedo, *const normal, *const roughness;
    unsigned int vao, vbo, ebo;
    int const nIndices;

public:
    Model(Program const &program, std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices,
          Texture const *albedo=nullptr, Texture const *normal=nullptr, Texture const *roughness=nullptr);
    Model(Model &&other);
    ~Model();
    Model(Model const &other) = delete;
    Model &operator=(Model const &other) = delete;

    void draw() const;
};
