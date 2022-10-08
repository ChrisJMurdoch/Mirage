
#pragma once

#include "aether/raytrace.hpp"

#include <memory>



class KDNode
{
protected:
    glm::vec3 const min, max;

public:
    KDNode(glm::vec3 const &min, glm::vec3 const &max);

public:
    static std::unique_ptr<KDNode> construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles);
    static std::unique_ptr<KDNode> construct(std::vector<raytrace::RayTri> const &triangles);
};



class KDNodeLeaf : public KDNode
{
private:
    std::vector<raytrace::RayTri> const triangles;

public:
    KDNodeLeaf(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles);

public:
    static std::unique_ptr<KDNodeLeaf> construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles);
};



class KDNodeParent : public KDNode
{
private:
    std::unique_ptr<KDNode> const left, right;
    
public:
    KDNodeParent(glm::vec3 const &min, glm::vec3 const &max, std::unique_ptr<KDNode> &&left, std::unique_ptr<KDNode> &&right);

public:
    static std::unique_ptr<KDNodeParent> construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles);
};



class KDTree
{
private:
    std::unique_ptr<KDNode> const root;

public:
    KDTree(std::vector<raytrace::RayTri> const &triangles);
};
