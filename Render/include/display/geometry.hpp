
#pragma once

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texturePosition;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    static void setAttributes();

private:
    static void setFloatAttr(unsigned int position, std::size_t size, std::size_t offset);
};
