
#include "utility/geometry.hpp"
#include "utility/objLoader.hpp"
#include "aether/image.hpp"

#include <iostream>

int run()
{
    std::cout << " // === AETHER === \\\\ " << std::endl << std::endl;

    Image image{"resources/models/gargoyle/textures/2k/albedo.jpg"};

    for (int y=0; y<image.getHeight(); y++)
        for (int x=0; x<image.getWidth(); x++)
            image[y][x].r *= 1.1f;
    
    image.save("resources/models/gargoyle/generated/2k/lightmap.jpg");

    std::cout << " \\\\ === AETHER === // " << std::endl;

    return 0;
}

int main()
{
    try
    {
        return run();
    }
    catch(std::exception const &e)
    {
        std::cerr << e.what() << '\n';
    }
}
