
#include "hexEngine/main.hpp"

#include "render/display.hpp"
#include "model/instance.hpp"
#include "arrayBuffer/arrayBuffer.hpp"
#include "hexEngine/hexCell.hpp"

#include <iostream>
#include <exception>

int main()
{
    try
    {
        PNC buffer(2);

        buffer.position(0)->xyz(3, 3, 3);
        buffer.normal(0)->xyz(0, 1, 0);
        buffer.colour(0)->xyz(250, 250, 250);
        buffer.position(1)->xyz(4, 4, 4);
        buffer.normal(1)->xyz(0.5f, 0, 0.5f);
        buffer.colour(1)->xyz(255, 255, 255);

        buffer.bufferData(0);

        // Create display and opengl context
        Display display(1000, 600, "WorldEngine");

        // Create programs
        Program post("shaders\\post.vert", "shaders\\post.frag");
        Program hex("shaders\\hex.vert", "shaders\\hex.frag");

        // Add post-processing to display
        display.addPostProgram(&post);

        // Create hex cell
        HexCell cell = HexCell();
        Model hexModel(&cell.getVertices(), &cell.getIndices(), &hex);

        // Add instances
        Instance hexInstance = Instance(hexModel);
        display.addInstance(&hexInstance);

        // Run display
        while (!display.shouldClose())
        {
            hexInstance.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);
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
