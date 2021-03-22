
#include "engine/main.hpp"

#include "render/display.hpp"
#include "model/instance.hpp"
#include "engine/planet.hpp"

#include <iostream>
#include <exception>

int main()
{
    try
    {
        // Create display and opengl context
        Display display(1000, 600, "WorldEngine");

        // Create programs
        Program post("shaders\\post.vert", "shaders\\post.frag");
        Program terrain("shaders\\terrain.vert", "shaders\\terrain.frag");

        // Add post-processing to display
        display.addPostProgram(&post);

        // Create planet
        const int terrainVertices = 600, noiseOctaves = 10;
        Planet planet(terrainVertices, noiseOctaves, 2);
        Model terrainModel(&planet.getVertices(), &planet.getIndices(), &terrain);

        // Add instances
        Instance planetInstance = Instance(terrainModel);
        display.addInstance(&planetInstance);

        // Run display
        while (!display.shouldClose())
        {
            post.setUniform3fv("planet_centre", glm::vec3(0.0f, 0.0f, 0.0f));
            planetInstance.physics();
            display.render();
        }

        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}
