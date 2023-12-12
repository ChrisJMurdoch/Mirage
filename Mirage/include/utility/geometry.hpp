
#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
    glm::vec3 tan;
    glm::vec3 btan;

    Vertex operator+(Vertex other) const
    {
        return Vertex{pos+other.pos, uv+other.uv, norm+other.norm, tan+other.tan, btan+other.btan};
    }

    Vertex operator-(Vertex other) const
    {
        return Vertex{pos-other.pos, uv-other.uv, norm-other.norm, tan-other.tan, btan-other.btan};
    }

    Vertex operator*(float x) const
    {
        return Vertex{pos*x, uv*x, norm*x, tan*x, btan*x};
    }
    
    Vertex operator/(float x) const
    {
        return Vertex{pos/x, uv/x, norm/x, tan/x, btan/x};
    }
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
