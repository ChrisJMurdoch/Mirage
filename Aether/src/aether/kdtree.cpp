
#include "aether/kdtree.hpp"

#include "aether/trAABBTest.hpp"

#include <limits>
#include <utility>
#include <iostream>

float constexpr FLOAT_MIN = std::numeric_limits<float>::min();
float constexpr FLOAT_MAX = std::numeric_limits<float>::max();

int constexpr MAX_LEAF_SIZE = 12; // TODO: Implement tree depth limit

// ===== Helper Functions =====

glm::vec3 componentMin(glm::vec3 a, glm::vec3 b)
{
    return glm::vec3{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

glm::vec3 componentMax(glm::vec3 a, glm::vec3 b)
{
    return glm::vec3{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

/// Tests if triangle and cuboid overlap.
bool overlaps(glm::vec3 const &min, glm::vec3 const &max, raytrace::RayTri const &triangle)
{
    glm::vec3 centre = (min+max)/2.0f;
    glm::vec3 halfSize = centre-min;
    float boxcenter[3] {centre.x, centre.y, centre.z};
    float boxhalfsize[3] {halfSize.x, halfSize.y, halfSize.z};
    float triverts[3][3]
    {
        {triangle.a.pos.x, triangle.a.pos.y, triangle.a.pos.z},
        {triangle.b.pos.x, triangle.b.pos.y, triangle.b.pos.z},
        {triangle.c.pos.x, triangle.c.pos.y, triangle.c.pos.z}
    };
    return triBoxOverlap(boxcenter, boxhalfsize, triverts);
}



// ===== KDNode =====

KDNode::KDNode(glm::vec3 const &min, glm::vec3 const &max)
    : min{min}, max{max}
{ }

std::unique_ptr<KDNode> KDNode::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
{
    return (triangles.size() > MAX_LEAF_SIZE) ?
        static_cast<std::unique_ptr<KDNode>>( KDNodeParent::construct(min, max, triangles) ) :
        static_cast<std::unique_ptr<KDNode>>( KDNodeLeaf::construct(min, max, triangles) );
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

std::pair<float, float> KDNode::tRange(raytrace::Ray const &ray) const
{
    glm::vec3 inv = 1.0f / ray.dir;
    float tmin, tmax;

    float tx1 = (min.x - ray.origin.x)*inv.x;
    float tx2 = (max.x - ray.origin.x)*inv.x;
    tmin = std::min(tx1, tx2);
    tmax = std::max(tx1, tx2);

    float ty1 = (min.y - ray.origin.y)*inv.y;
    float ty2 = (max.y - ray.origin.y)*inv.y;
    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = (min.z - ray.origin.z)*inv.z;
    float tz2 = (max.z - ray.origin.z)*inv.z;
    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return {tmin, tmax};
}

std::optional<float> KDNode::intersect(raytrace::Ray const &ray) const
{
    auto bounds = tRange(ray);
    float tmin = bounds.first, tmax = bounds.second;

    if (tmax < tmin)
        return {};
    
    if (tmax < 0.0f)
        return {};
    
    return std::max(0.0f, tmin);
}



// ===== KDNodeLeaf =====

KDNodeLeaf::KDNodeLeaf(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
    : KDNode{min, max}, triangles{triangles}
{ }

std::unique_ptr<KDNodeLeaf> KDNodeLeaf::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
{
    return std::make_unique<KDNodeLeaf>(min, max, triangles);
}


std::optional<raytrace::TriHit> KDNodeLeaf::getHit(raytrace::Ray const &ray, float tMin) const
{
    auto bounds = this->tRange(ray); // TODO: Optimise by passing bounds into tri calc for early termination
    std::optional<raytrace::TriHit> closestHit = {};
    for (raytrace::RayTri const &tri : triangles)
    {
        float mT = closestHit ? closestHit->getT() : tMin;
        std::optional<raytrace::Hit> hit = tri.getHit(ray, mT);
        if ( hit && (hit->getT()>bounds.first && hit->getT()<bounds.second) && (!closestHit || hit->getT() < closestHit->getT()) )
            closestHit.emplace( raytrace::TriHit{*hit, &tri} );
    }
    return closestHit;
}



// ===== KDNodeParent =====

KDNodeParent::KDNodeParent(glm::vec3 const &min, glm::vec3 const &max, std::unique_ptr<KDNode> &&left, std::unique_ptr<KDNode> &&right)
    : KDNode{min, max}, left{std::move(left)}, right{std::move(right)}
{ }

std::unique_ptr<KDNodeParent> KDNodeParent::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<raytrace::RayTri> const &triangles)
{
    // Get bounding box dimension
    glm::vec3 dim = max-min;

    // Pivot halfway along longest dimension - TODO: Optimise pivot location (and look into shrinking bounds after allocation)
    glm::vec3 pivotMin, pivotMax;
    glm::vec3 middle = (min+max) / 2.0f;
    if ( dim.x>dim.y && dim.x>dim.z )
    {
        pivotMin = glm::vec3{ middle.x, min.y, min.z };
        pivotMax = glm::vec3{ middle.x, max.y, max.z };
    }
    else if ( dim.y>dim.z )
    {
        pivotMin = glm::vec3{ min.x, middle.y, min.z };
        pivotMax = glm::vec3{ max.x, middle.y, max.z };
    }
    else
    {
        pivotMin = glm::vec3{ min.x, min.y, middle.z };
        pivotMax = glm::vec3{ max.x, max.y, middle.z };
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

std::optional<raytrace::TriHit> KDNodeParent::getHit(raytrace::Ray const &ray, float tMin) const
{
    // Get child box intersections (minimum tval)
    std::optional<float> leftIntersect = left->intersect(ray);
    std::optional<float> rightIntersect = right->intersect(ray);

    // No intersections
    if (!leftIntersect && !rightIntersect)
        return {};
    
    // Find close and far boxes
    bool leftCloser = leftIntersect && ( !rightIntersect || (*leftIntersect) < (*rightIntersect) );
    std::unique_ptr<KDNode> const &close = leftCloser ? left : right;
    std::unique_ptr<KDNode> const &far   = leftCloser ? right : left;

    // Check for collision in close box
    std::optional<raytrace::TriHit> closeHit = close->getHit(ray, tMin);
    if (closeHit)
        return closeHit;

    // Check for collision in far box
    std::optional<float> const &farIntersect = leftCloser ? rightIntersect : leftIntersect;
    return farIntersect ? far->getHit(ray, tMin) : std::nullopt;
}



// ===== KDTree =====

KDTree::KDTree(std::vector<raytrace::RayTri> const &triangles)
    : root{KDNode::construct(triangles)}
{ }

std::optional<raytrace::TriHit> KDTree::getHit(raytrace::Ray const &ray) const
{
    return root->getHit(ray, FLOAT_MAX);
}
