
#pragma once

struct Material;

namespace mtlLoader
{
    /// Loads and processes .mtl file and returns a Material with the relevant textures
    Material loadMtl(char const *filepath, bool const verbose=false);
}
