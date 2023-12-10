
#include "aether/raytrace.hpp"

#include "aether/kdtree.hpp"
#include "aether/image.hpp"



// ===== RayTri =====

RayTri::RayTri(Vertex const &a, Vertex const &b, Vertex const &c, PhysicalMaterial &material)
    : a{a}, b{b}, c{c}, material{material}
{
    // Pre-calculate plane normal
    glm::vec3 ab = b.pos-a.pos;
    glm::vec3 ac = c.pos-a.pos;
    norm = glm::cross(ab, ac);

    // Pre-calculate distance from origin
    d = -glm::dot(norm, a.pos);

    // Pre-calculate edges 
    v12 = b.pos-a.pos;
    v23 = c.pos-b.pos;
    v31 = a.pos-c.pos;
    
    // Pre-calculate real surface area
    float realSurfaceArea = 0.5f * glm::length(norm);

    // Pre-calculate texture surface area
    glm::vec2 uvAb=b.uv-a.uv, uvAc=c.uv-a.uv;
    float uvSurfaceArea = 0.5f * std::abs(uvAb.x*uvAc.y - uvAb.y*uvAc.x);
    int texturePixels = material.lightmap->getWidth() * material.lightmap->getHeight();
    float textureSurfaceArea = texturePixels * uvSurfaceArea;

    // Set light modifier based on real and texture areas (accounts for per-texel ray hit frequency)
    lightModifier = textureSurfaceArea / realSurfaceArea;
}

Vertex RayTri::interpolate(glm::vec3 const &p) const
{
    // Barycentric coordinate interpolation from: https://gamedev.stackexchange.com/a/23745
    glm::vec3 v2=c.pos-a.pos, v3=p-a.pos;
    float d00 = glm::dot(v12, v12);
    float d01 = glm::dot(v12, v2);
    float d11 = glm::dot(v2, v2);
    float d20 = glm::dot(v3, v12);
    float d21 = glm::dot(v3, v2);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1 - v - w;
    return a*u + b*v + c*w;
}



// ===== RayScene =====

std::vector<RayTri> getTriangles(std::vector<std::pair<Mesh const, PhysicalMaterial &>> const &meshes)
{
    std::vector<RayTri> triangles;
    for (std::pair<Mesh const, PhysicalMaterial &> const &pair : meshes)
    {
        Mesh const &mesh = pair.first;
        PhysicalMaterial &image = pair.second;
        for (int i=0; i<mesh.indices.size(); i+=3)
            triangles.push_back( RayTri{ mesh.vertices[mesh.indices[i+0]], mesh.vertices[mesh.indices[i+1]], mesh.vertices[mesh.indices[i+2]], image } );
    }
    return triangles;
}

RayScene::RayScene(std::vector<std::pair<Mesh const, PhysicalMaterial &>> const &meshes) : kdtree{ new KDTree(getTriangles(meshes)) }
{ }

RayScene::~RayScene()
{
    delete kdtree;
}

std::optional<Hit> RayScene::getHit(Ray const &ray) const
{
    return kdtree->getHit(ray);
}
