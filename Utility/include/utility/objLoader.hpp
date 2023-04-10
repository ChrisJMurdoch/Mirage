
#pragma once

#include <vector>

struct Mesh;
struct Vertex;

namespace objLoader
{
    /// Loads and processes .obj file and returns pair<vertices, indices>
    Mesh loadObj(char const *filepath, float const scale=1.0f, bool const verbose=false);

    /// Generates tangents and bitangents for given vertex array
    void generateTangents(std::vector<unsigned int> const &indices, std::vector<Vertex> &vertices);

    /// Generates normals for given vertex array
    void generateNormals(std::vector<unsigned int> const &indices, std::vector<Vertex> &vertices);
}
