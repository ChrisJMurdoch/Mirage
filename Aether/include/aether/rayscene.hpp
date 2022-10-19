
#pragma once

#include "aether/kdtree.hpp"

struct Mesh;
class Image;
struct Hit;
struct Ray;

/// Collidable scene
class RayScene
{
private:
    KDTree kdtree;

public:
    RayScene(std::vector<std::pair<Mesh const, Image &>> const &meshes);
    std::optional<Hit> getHit(Ray const &ray) const;
};
