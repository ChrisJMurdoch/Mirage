
#include "aether/kdtree.hpp"

#include <limits>
#include <utility>
#include <iostream>

float constexpr FLOAT_MIN = std::numeric_limits<float>::min();
float constexpr FLOAT_MAX = std::numeric_limits<float>::max();

int constexpr MAX_LEAF_SIZE = 100; //  TODO: see why MAX_LEAF_SIZE<10 crashes

// ===== Helper Functions =====

glm::vec3 componentMin(glm::vec3 a, glm::vec3 b)
{
    return glm::vec3{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

glm::vec3 componentMax(glm::vec3 a, glm::vec3 b)
{
    return glm::vec3{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

/// Tests if cuboids overlap.
bool overlaps(glm::vec3 const &minA, glm::vec3 const &maxA, glm::vec3 const &minB, glm::vec3 const &maxB)
{
    return maxA.x>minB.x && minA.x<maxB.x &&
           maxA.y>minB.y && minA.y<maxB.y &&
           maxA.z>minB.z && minA.z<maxB.z;
}

/// Tests if triangle and cuboid overlap.
/// May return false positive but won't return false negative - this is fine for bound partitioning
/// TODO: optimise
bool overlaps(glm::vec3 const &min, glm::vec3 const &max, raytrace::RayTri const &triangle)
{
    // Get bounding box of triangle
    glm::vec3 triMin = componentMin(triangle.a.pos, componentMin(triangle.b.pos, triangle.c.pos));
    glm::vec3 triMax = componentMax(triangle.a.pos, componentMax(triangle.b.pos, triangle.c.pos));

    // Check intersection of bounding boxes
    return overlaps(min, max, triMin, triMax);
}



// ===== KDNode =====

KDNode::KDNode(glm::vec3 const &min, glm::vec3 const &max)
    : min{min}, max{max}
{ }

std::unique_ptr<KDNode> KDNode::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
{
    return (triangles.size()<MAX_LEAF_SIZE) ?
        static_cast<std::unique_ptr<KDNode>>( KDNodeLeaf::construct(min, max, triangles)   ) :
        static_cast<std::unique_ptr<KDNode>>( KDNodeParent::construct(min, max, triangles) );
}

std::unique_ptr<KDNode> KDNode::construct(std::vector<raytrace::RayTri> const &triangles)
{
    // Calculate min and max
    glm::vec3 min{FLOAT_MAX}, max{FLOAT_MIN};
    for (raytrace::RayTri const &tri : triangles)
    {
        min = componentMin( componentMin(min, tri.a.pos), componentMin(tri.b.pos, tri.c.pos) );
        max = componentMax( componentMax(max, tri.a.pos), componentMax(tri.b.pos, tri.c.pos) );
    }

    // Construct node
    return KDNode::construct(min, max, triangles);
}



// ===== KDNodeLeaf =====

KDNodeLeaf::KDNodeLeaf(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
    : KDNode{min, max}, triangles{triangles}
{ }

std::unique_ptr<KDNodeLeaf> KDNodeLeaf::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
{
    return std::make_unique<KDNodeLeaf>(min, max, triangles);
}



// ===== KDNodeParent =====

KDNodeParent::KDNodeParent(glm::vec3 const &min, glm::vec3 const &max, std::unique_ptr<KDNode> &&left, std::unique_ptr<KDNode> &&right)
    : KDNode{min, max}, left{std::move(left)}, right{std::move(right)}
{ }

std::unique_ptr<KDNodeParent> KDNodeParent::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
{
    // Get bounding box dimension
    glm::vec3 dim = max-min;

    // Pivot halfway along longest dimension - TODO: optimise
    glm::vec3 pivotMin, pivotMax;
    if ( dim.x>dim.y && dim.x>dim.z )
    {
        pivotMin = glm::vec3{ (min.x+max.x)/2.0f, min.y, min.z };
        pivotMax = glm::vec3{ (min.x+max.x)/2.0f, max.y, max.z };
    }
    else if ( dim.y>dim.z )
    {
        pivotMin = glm::vec3{ min.x, (min.y+max.y)/2.0f, min.z };
        pivotMax = glm::vec3{ max.x, (min.y+max.y)/2.0f, max.z };
    }
    else
    {
        pivotMin = glm::vec3{ min.x, min.y, (min.z+max.z)/2.0f };
        pivotMax = glm::vec3{ max.x, max.y, (min.z+max.z)/2.0f };
    }

    // Partition triangles
    std::vector<raytrace::RayTri> leftTriangles, rightTriangles;
    for (raytrace::RayTri const &tri : triangles)
    {
        if (overlaps(min, pivotMax, tri))
            leftTriangles.push_back(tri);
        if (overlaps(pivotMin, max, tri))
            rightTriangles.push_back(tri);
    }

    // Recursively construct child nodes
    return std::make_unique<KDNodeParent>( min, max, KDNode::construct(min, pivotMax, leftTriangles), KDNode::construct(pivotMin, max, rightTriangles) );
}



// ===== KDTree =====

KDTree::KDTree(std::vector<raytrace::RayTri> const &triangles)
    : root{KDNode::construct(triangles)}
{ }
