
#include "render/display.hpp"
#include "model/instance.hpp"
#include "hexEngine/hexCell.hpp"
#include "arrayBuffer/vertexBuffer.hpp"

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
        Program hex("shaders\\hex.vert", "shaders\\hex.frag");

        // Add post-processing to display
        display.addPostProgram(&post);

        // Create instances
        PO instances(3);
        instances.position(0)->xyz(0.0f, 0.0f,  0.0f);
        instances.position(1)->xyz(0.0f, 0.0f,  1.0f);
        instances.position(2)->xyz(0.0f, 0.0f, -1.0f);

        // Create hex cell
        HexCell cell = HexCell();
        Model hexModel(&cell.getVertices(), &cell.getIndices(), &instances, &hex);

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
