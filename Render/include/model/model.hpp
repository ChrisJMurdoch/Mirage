
#pragma once

#include <vector>

class Program;
class Texture;

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };

struct Vertex
{
    Vec3 position;
    Vec2 texturePosition;
    static void setAttributes();
};

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
