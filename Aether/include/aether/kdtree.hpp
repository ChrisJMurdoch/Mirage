
#pragma once

#include "aether/rayGeometry.hpp"

#include <memory>
#include <optional>



class KDNode
{
public:
    glm::vec3 const min, max;

public:
    KDNode(glm::vec3 const &min, glm::vec3 const &max);

public:
    static std::unique_ptr<KDNode> construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles);
    static std::unique_ptr<KDNode> construct(std::vector<RayTri> const &triangles);

    std::pair<float, float> tRange(Ray const &ray) const;
    std::optional<float> intersect(Ray const &ray) const;
    virtual std::optional<Hit> getHit(Ray const &ray, float tMin) const = 0;
};



class KDNodeLeaf : public KDNode
{
private:
    std::vector<RayTri> const triangles;

public:
    KDNodeLeaf(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles);

public:
    static std::unique_ptr<KDNodeLeaf> construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles);
    virtual std::optional<Hit> getHit(Ray const &, float tMin) const override;
};



class KDNodeParent : public KDNode
{
private:
    std::unique_ptr<KDNode> const left, right;
    
public:
    KDNodeParent(glm::vec3 const &min, glm::vec3 const &max, std::unique_ptr<KDNode> &&left, std::unique_ptr<KDNode> &&right);

public:
    static std::unique_ptr<KDNodeParent> construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles);
    virtual std::optional<Hit> getHit(Ray const &ray, float tMin) const override;
};



class KDTree
{
private:
    std::unique_ptr<KDNode> const root;

public:
    KDTree(std::vector<RayTri> const &triangles);
    std::optional<Hit> getHit(Ray const &ray) const;
};
