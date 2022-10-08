
#pragma once

#include "utility/geometry.hpp"

#include <optional>

class Image;

namespace raytrace
{
    struct RayTri;
    class RayMesh;

    /// Ray with origin and direction
    struct Ray
    {
        glm::vec3 const origin, dir;

        glm::vec3 at(float t) const
        {
            return origin + dir*t;
        }
    };

    /// Represents a ray collision
    struct Hit
    {
        float t;
        float &getT() { return t; }
    };

    /// Represents a ray collision and stores the triangle collided with
    struct TriHit
    {
        Hit hit;
        RayTri const *triangle;
        float &getT() { return hit.getT(); }
    };

    /// Represents a triangle collision and stores the mesh collided with
    struct MeshHit
    {
        TriHit triHit;
        RayMesh const *mesh;
        float &getT() { return triHit.getT(); }
    };

    /// Collidable triangle
    struct RayTri
    {
        // Corner vertices
        Vertex const a, b, c;

        // Pre-calculated values needed for intersection calculation
        glm::vec3 norm;
        float d;
        glm::vec3 v12, v23, v31;

        RayTri(Vertex const &a, Vertex const &b, Vertex const &c);
        Vertex interpolate(glm::vec3 const &point) const;
        std::optional<Hit> getHit(Ray const &ray, float maxT) const;
    };
    
    /// Collidable mesh
    class RayMesh
    {
    private:
        std::vector<RayTri> triangles;
        Image &lightmap;

    public:
        RayMesh(Mesh const &mesh, Image &lightmap);
        std::optional<TriHit> getHit(Ray const &ray, float maxT) const;
        Image &getLightmap() const;
    };

    /// Collidable scene
    class RayScene
    {
    private:
        std::vector<RayMesh> const &rayMeshes;

    public:
        RayScene(std::vector<RayMesh> const &meshes);
        std::optional<MeshHit> getHit(Ray const &ray) const;
    };
}
