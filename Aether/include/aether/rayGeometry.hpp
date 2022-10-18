
#pragma once

#include "utility/geometry.hpp"

#include <optional>
#include <memory>

class Image;
class KDTree;
struct RayTri;
struct RayMesh;

/// Ray with origin and direction
struct Ray
{
    glm::vec3 const origin, dir;

    glm::vec3 at(float t) const
    {
        return origin + dir*t;
    }
};

/// Represents a ray collision and stores the triangle collided with
struct TriHit
{
    float t;
    RayTri const *triangle;
    float &getT() { return t; }
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
    std::optional<TriHit> getHit(Ray const &ray, float maxT) const;
    Vertex interpolate(glm::vec3 const &point) const;
};

/// Tuple of mesh and corresponding lightmap used as argument for RayScene
struct RayMesh
{
    Mesh const mesh;
    Image &lightmap;
};

/// Collidable scene
class RayScene
{
private:
    KDTree *kdtree;

public:
    RayScene(std::vector<RayMesh> const &meshes);
    ~RayScene();
    std::optional<TriHit> getHit(Ray const &ray) const;
};
