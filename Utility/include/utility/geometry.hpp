
#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texturePosition;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
