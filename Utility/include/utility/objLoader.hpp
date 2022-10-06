
#pragma once

struct Mesh;

namespace objLoader
{
    /// Loads and processes .obj file and returns pair<vertices, indices>
    Mesh loadObj(char const *filepath, bool const verbose=false);
}
