
#pragma once

struct Mesh;

namespace fbxLoader
{
    /// Loads and processes .fbx file and returns a Mesh object
    Mesh loadFbx(char const *filepath, float const scale=1.0f, bool const verbose=false);
}
