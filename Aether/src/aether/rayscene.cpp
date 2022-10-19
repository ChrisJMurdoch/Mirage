
#include "aether/rayscene.hpp"

#include "utility/geometry.hpp"
#include "aether/image.hpp"
#include "aether/rayGeometry.hpp"

std::vector<RayTri> getTriangles(std::vector<std::pair<Mesh const, Image &>> const &meshes)
{
    std::vector<RayTri> triangles;
    for (std::pair<Mesh const, Image &> const &pair : meshes)
    {
        Mesh const &mesh = pair.first;
        Image &image = pair.second;
        for (int i=0; i<mesh.indices.size(); i+=3)
            triangles.push_back( RayTri{ mesh.vertices[mesh.indices[i+0]], mesh.vertices[mesh.indices[i+1]], mesh.vertices[mesh.indices[i+2]], image } );
    }
    return triangles;
}

RayScene::RayScene(std::vector<std::pair<Mesh const, Image &>> const &meshes) : kdtree{ new KDTree(getTriangles(meshes)) }
{ }

RayScene::~RayScene()
{
    delete kdtree; // TODO: Change to unique_ptr/composition
}

std::optional<Hit> RayScene::getHit(Ray const &ray) const
{
    return kdtree->getHit(ray);
}
