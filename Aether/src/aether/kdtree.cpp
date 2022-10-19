
#include "aether/kdtree.hpp"

#include "aether/triAABBTest.hpp"

#include <limits>

float constexpr FLOAT_MIN = std::numeric_limits<float>::min();
float constexpr FLOAT_MAX = std::numeric_limits<float>::max();

int constexpr MAX_LEAF_SIZE = 12; // TODO: Implement tree depth limit



// ===== Helper Functions =====

inline glm::vec3 componentMin(glm::vec3 a, glm::vec3 b)
{
    return glm::vec3{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

inline glm::vec3 componentMax(glm::vec3 a, glm::vec3 b)
{
    return glm::vec3{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

/// Tests if triangle and cuboid overlap.
bool overlaps(glm::vec3 const &min, glm::vec3 const &max, RayTri const &triangle)
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

std::optional<Hit> getRayTriHit(RayTri const &rayTri, Ray const &ray, std::pair<float, float> tBounds)
{
    // Return if plane and ray are parallel
    float normDotRayDir = glm::dot(rayTri.norm, ray.dir);
    if (fabs(normDotRayDir) < FLOAT_MIN)
        return {};
    
    // Compute t
    float t = -(glm::dot(rayTri.norm, ray.origin) + rayTri.d) / normDotRayDir;
 
    // Return if point is behind origin or is more than maxT (short-circuit if would be behind previously calculated triangle)
    if (t < tBounds.first || t > tBounds.second)
        return {};
 
    // Compute intersection point
    glm::vec3 p = ray.at(t);

    // Check if correct side of edge AB
    glm::vec3 vp0 = p - rayTri.a.pos;
    if (glm::dot(rayTri.norm, glm::cross(rayTri.v12, vp0)) < 0)
        return {};
 
    // Check if correct side of edge BC
    glm::vec3 vp1 = p - rayTri.b.pos;
    if (glm::dot(rayTri.norm, glm::cross(rayTri.v23, vp1)) < 0)
        return {};
 
    // Check if correct side of edge CA
    glm::vec3 vp2 = p - rayTri.c.pos;
    if (glm::dot(rayTri.norm, glm::cross(rayTri.v31, vp2)) < 0)
        return {};
    
    // Hit
    return Hit{t, &rayTri};
}



// ===== KDNode =====

KDNode::KDNode(glm::vec3 const &min, glm::vec3 const &max)
    : min{min}, max{max}
{ }

std::unique_ptr<KDNode> KDNode::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles)
{
    // Not 100% sure about this optimisation
    // Might introduce bugs but gives a +42% speedup
    // Shrinks bounding box to fit elements
    glm::vec3 shrunkMin{FLOAT_MAX}, shrunkMax{FLOAT_MIN};
    for (RayTri const &tri : triangles)
    {
        shrunkMin = componentMin( componentMin(shrunkMin, tri.a.pos), componentMin(tri.b.pos, tri.c.pos) );
        shrunkMax = componentMax( componentMax(shrunkMax, tri.a.pos), componentMax(tri.b.pos, tri.c.pos) );
    }
    shrunkMin = componentMax(shrunkMin, min);
    shrunkMax = componentMin(shrunkMax, max);

    // Select relevant constructor
    return (triangles.size() > MAX_LEAF_SIZE) ?
        static_cast<std::unique_ptr<KDNode>>( KDNodeParent::construct(shrunkMin, shrunkMax, triangles) ) :
        static_cast<std::unique_ptr<KDNode>>( KDNodeLeaf::construct(shrunkMin, shrunkMax, triangles) );
}

std::unique_ptr<KDNode> KDNode::construct(std::vector<RayTri> const &triangles)
{
    // Calculate min and max
    glm::vec3 min{FLOAT_MAX}, max{FLOAT_MIN};
    for (RayTri const &tri : triangles)
    {
        min = componentMin( componentMin(min, tri.a.pos), componentMin(tri.b.pos, tri.c.pos) );
        max = componentMax( componentMax(max, tri.a.pos), componentMax(tri.b.pos, tri.c.pos) );
    }

    // Construct node
    return KDNode::construct(min, max, triangles);
}

/// Find min and max t value for ray-AABB intersection.  Adapted from 2D implementation: https://tavianator.com/2011/ray_box.html
std::optional<std::pair<float, float>> KDNode::intersection(Ray const &ray) const
{
    float tmin, tmax;

    float tx1 = (min.x - ray.origin.x)*ray.dirInv.x;
    float tx2 = (max.x - ray.origin.x)*ray.dirInv.x;
    tmin = std::min(tx1, tx2);
    tmax = std::max(tx1, tx2);

    float ty1 = (min.y - ray.origin.y)*ray.dirInv.y;
    float ty2 = (max.y - ray.origin.y)*ray.dirInv.y;
    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = (min.z - ray.origin.z)*ray.dirInv.z;
    float tz2 = (max.z - ray.origin.z)*ray.dirInv.z;
    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    if (tmax < tmin)
        return {};
    
    if (tmax < 0.0f)
        return {};
    
    return {{std::max(0.0f, tmin), tmax}};
}



// ===== KDNodeLeaf =====

KDNodeLeaf::KDNodeLeaf(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles)
    : KDNode{min, max}, triangles{triangles}
{ }

std::unique_ptr<KDNodeLeaf> KDNodeLeaf::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles)
{
    return std::make_unique<KDNodeLeaf>(min, max, triangles);
}

std::optional<Hit> KDNodeLeaf::getHit(Ray const &ray, std::pair<float, float> tBounds) const
{
    std::optional<Hit> closestHit = {};
    for (RayTri const &tri : triangles)
    {
        float tMin = closestHit ? closestHit->t : tBounds.first;
        std::optional<Hit> hit = getRayTriHit(tri, ray, {tMin, tBounds.second});
        if (hit)
            closestHit.emplace(*hit);
    }
    return closestHit;
}



// ===== KDNodeParent =====

KDNodeParent::KDNodeParent(glm::vec3 const &min, glm::vec3 const &max, std::unique_ptr<KDNode> &&left, std::unique_ptr<KDNode> &&right)
    : KDNode{min, max}, left{std::move(left)}, right{std::move(right)}
{ }

std::unique_ptr<KDNodeParent> KDNodeParent::construct(glm::vec3 const &min, glm::vec3 const &max, std::vector<RayTri> const &triangles)
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
    std::vector<RayTri> leftTriangles, rightTriangles;
    for (RayTri const &tri : triangles)
    {
        if (overlaps(min, pivotMax, tri))
            leftTriangles.push_back(tri);
        if (overlaps(pivotMin, max, tri))
            rightTriangles.push_back(tri);
    }

    // Recursively construct child nodes
    return std::make_unique<KDNodeParent>( min, max, KDNode::construct(min, pivotMax, leftTriangles), KDNode::construct(pivotMin, max, rightTriangles) );
}

std::optional<Hit> KDNodeParent::getHit(Ray const &ray, std::pair<float, float> tBounds) const
{
    // Get child box intersections (minimum tval)
    std::optional<std::pair<float, float>> leftIntersect = left->intersection(ray);
    std::optional<std::pair<float, float>> rightIntersect = right->intersection(ray);

    // No intersections
    if (!leftIntersect && !rightIntersect)
        return {};
    
    // Find close and far boxes
    bool leftCloser = leftIntersect && ( !rightIntersect || leftIntersect->first < rightIntersect->first );
    std::unique_ptr<KDNode> const &close = leftCloser ? left : right;
    std::unique_ptr<KDNode> const &far   = leftCloser ? right : left;

    // Check for collision in close box
    std::optional<std::pair<float, float>> const &closeIntersect = leftCloser ? leftIntersect : rightIntersect;
    std::optional<Hit> closeHit = close->getHit(ray, *closeIntersect);
    if (closeHit)
        return closeHit;

    // Check for collision in far box
    std::optional<std::pair<float, float>> const &farIntersect = leftCloser ? rightIntersect : leftIntersect;
    return farIntersect ? far->getHit(ray, *farIntersect) : std::nullopt;
}



// ===== KDTree =====

KDTree::KDTree(std::vector<RayTri> const &triangles)
    : root{KDNode::construct(triangles)}
{ }

std::optional<Hit> KDTree::getHit(Ray const &ray) const
{
    return root->getHit(ray, {FLOAT_MIN, FLOAT_MAX});
}
