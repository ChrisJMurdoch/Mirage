
#pragma once

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texturePosition;
    glm::vec3 normal;
    static void setAttributes();
};
