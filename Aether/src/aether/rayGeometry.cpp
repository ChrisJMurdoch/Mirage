
#include "aether/rayGeometry.hpp"

#include "aether/kdtree.hpp"

#include <limits>
#include <iostream>

float constexpr FLOAT_MIN = std::numeric_limits<float>::min();
float constexpr FLOAT_MAX = std::numeric_limits<float>::max();



// ===== RayTri =====

RayTri::RayTri(Vertex const &a, Vertex const &b, Vertex const &c, Image &lightmap)
    : a{a}, b{b}, c{c}, lightmap{lightmap}
{
    // Pre-calculate plane normal
    glm::vec3 ab = b.pos-a.pos;
    glm::vec3 ac = c.pos-a.pos;
    norm = glm::cross(ab, ac);

    // Pre-calculate distance from origin
    d = -glm::dot(norm, a.pos);

    // Pre-calculate edges 
    v12 = b.pos-a.pos;
    v23 = c.pos-b.pos;
    v31 = a.pos-c.pos;
}

Vertex RayTri::interpolate(glm::vec3 const &p) const
{
    // Barycentric coordinate interpolation from: https://gamedev.stackexchange.com/a/23745
    glm::vec3 v2=c.pos-a.pos, v3=p-a.pos;
    float d00 = glm::dot(v12, v12);
    float d01 = glm::dot(v12, v2);
    float d11 = glm::dot(v2, v2);
    float d20 = glm::dot(v3, v12);
    float d21 = glm::dot(v3, v2);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1 - v - w;
    return a*u + b*v + c*w;
}
