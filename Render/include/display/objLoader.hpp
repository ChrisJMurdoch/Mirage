
#pragma once

#include <utility>
#include <vector>

struct Vertex;

namespace objLoader
{
    /// Loads and processes .obj file and returns pair<vertices, indices>
    std::pair<std::vector<Vertex>, std::vector<unsigned int>> loadObj(char const *filepath);
}
