
#pragma once

#include "utility/geometry.hpp"

#include <optional>

struct Mesh;
struct PhysicalMaterial;
class KDTree;

/// Ray with origin and direction
struct Ray
{
    glm::vec3 const origin, dir, dirInv;

    Ray(glm::vec3 origin, glm::vec3 dir) : origin{origin}, dir{dir}, dirInv{1.0f/dir}
    { }

    glm::vec3 at(float t) const
    {
        return origin + dir*t;
    }
};

/// Collidable triangle
struct RayTri
{
    Vertex const a, b, c;
    PhysicalMaterial &material;

    // Pre-calculated values needed for intersection calculation
    glm::vec3 norm;
    float d;
    glm::vec3 v12, v23, v31;

    // Pre-calculated light modifier
    float lightModifier;

    RayTri(Vertex const &a, Vertex const &b, Vertex const &c, PhysicalMaterial &material);
    Vertex interpolate(glm::vec3 const &point) const;
};

/// Represents a ray collision and stores the triangle collided with
struct Hit
{
    float t;
    RayTri const *triangle;
};

/// Collidable scene
class RayScene
{
private:
    KDTree *kdtree;

public:
    RayScene(std::vector<std::pair<Mesh const, PhysicalMaterial &>> const &meshes);
    ~RayScene();
    RayScene(RayScene const &other) = delete;
    RayScene &operator=(RayScene const &other) = delete;
    std::optional<Hit> getHit(Ray const &ray) const;
};
