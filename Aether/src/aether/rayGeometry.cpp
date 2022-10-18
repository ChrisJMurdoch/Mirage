
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

std::optional<Hit> RayTri::getHit(Ray const &ray, float maxT) const
{
    // Return if plane and ray are parallel
    float normDotRayDir = glm::dot(norm, ray.dir);
    if (fabs(normDotRayDir) < FLOAT_MIN)
        return {};
    
    // Compute t
    float t = -(glm::dot(norm, ray.origin) + d) / normDotRayDir;
 
    // Return if point is behind origin or is more than maxT (short-circuit if would be behind previously calculated triangle)
    if (t < 0 || t >= maxT)
        return {};
 
    // Compute intersection point
    glm::vec3 p = ray.at(t);

    // Check if correct side of edge AB
    glm::vec3 vp0 = p - a.pos;
    if (glm::dot(norm, glm::cross(v12, vp0)) < 0)
        return {};
 
    // Check if correct side of edge BC
    glm::vec3 vp1 = p - b.pos;
    if (glm::dot(norm, glm::cross(v23, vp1)) < 0)
        return {};
 
    // Check if correct side of edge CA
    glm::vec3 vp2 = p - c.pos;
    if (glm::dot(norm, glm::cross(v31, vp2)) < 0)
        return {};
    
    // Hit
    return Hit{t, this};
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



// ===== RayScene =====

std::vector<RayTri> getTriangles(std::vector<std::pair<Mesh const, Image &>> const &meshes)
{
    std::vector<RayTri> triangles;
    for (std::pair<Mesh const, Image &> const &pair : meshes)
    {
        Mesh const &mesh = pair.first;
        Image &image = pair.second;
        for (int i=0; i<mesh.indices.size(); i+=3)
            triangles.push_back( RayTri{ mesh.vertices[mesh.indices[i+0]], mesh.vertices[mesh.indices[i+1]], mesh.vertices[mesh.indices[i+2]], image } );
    }
    return triangles;
}

RayScene::RayScene(std::vector<std::pair<Mesh const, Image &>> const &meshes) : kdtree{ new KDTree(getTriangles(meshes)) }
{ }

RayScene::~RayScene()
{
    delete kdtree; // TODO: Change to unique_ptr/composition
}

std::optional<Hit> RayScene::getHit(Ray const &ray) const
{
    return kdtree->getHit(ray);
}
