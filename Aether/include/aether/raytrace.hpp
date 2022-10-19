
#pragma once

#include "utility/geometry.hpp"

#include <optional>

struct Mesh;
class Image;
class KDTree;

/// Ray with origin and direction
struct Ray
{
    glm::vec3 const origin, dir;

    glm::vec3 at(float t) const
    {
        return origin + dir*t;
    }
};

/// Collidable triangle
struct RayTri
{
    Vertex const a, b, c;
    Image &lightmap;

    // Pre-calculated values needed for intersection calculation
    glm::vec3 norm;
    float d;
    glm::vec3 v12, v23, v31;

    RayTri(Vertex const &a, Vertex const &b, Vertex const &c, Image &lightmap);
    Vertex interpolate(glm::vec3 const &point) const;
};

/// Represents a ray collision and stores the triangle collided with
struct Hit
{
    float t;
    RayTri const *triangle;
    float &getT() { return t; }
};

/// Collidable scene
class RayScene
{
private:
    KDTree *kdtree;

public:
    RayScene(std::vector<std::pair<Mesh const, Image &>> const &meshes);
    ~RayScene();
    std::optional<Hit> getHit(Ray const &ray) const;
};
