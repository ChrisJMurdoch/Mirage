
#include "render/display.hpp"
#include "model/instance.hpp"
#include "hexEngine/hexCell.hpp"
#include "arrayBuffer/vertexBuffer.hpp"

#include <iostream>
#include <exception>
#include <random>

static std::random_device device;
static std::mt19937 twister(device());
static std::uniform_real_distribution<float> uniform(0.0f, 0.1f);

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
        int const gridDepth = 3, gridHeight = 3;
        Pos instances(gridDepth*gridHeight);
        float const xOrigin = -(gridDepth-1)/2.0f, yOrigin = -(gridHeight-1)/2.0f;
        for (int z=0; z<gridHeight; z++)
        {
            for (int x=0; x<gridDepth; x++)
            {
                int const index = (z*gridDepth)+x;
                float const xVal = (xOrigin+x)*1.5f*HexCell::RADIUS;
                float const zVal = (x%2==0) ? (yOrigin+z+0.5f)*2*HexCell::APOTHEM : (yOrigin+z)*2*HexCell::APOTHEM;
                instances.position(index)->xyz(xVal, uniform(twister), zVal);
                std::cout << index << ", " << xVal << ", " << zVal << std::endl;
            }
        }

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
