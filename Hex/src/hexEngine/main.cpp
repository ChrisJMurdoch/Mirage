
#include "render/display.hpp"
#include "model/instance.hpp"
#include "hexEngine/hexCell.hpp"
#include "arrayBuffer/vertexBuffer.hpp"
#include "generate/noise.hpp"

#include <iostream>
#include <exception>
#include <random>
#include <algorithm>

static std::random_device device;
static std::mt19937 twister(device());
static std::uniform_real_distribution<float> uniform(1.0f, 1.3f);

int main()
{
    try
    {
        // Create display and opengl context
        Display display(1000, 600, "WorldEngine");

        // Create programs
        Program post("Hex\\shaders\\post.vert", "Hex\\shaders\\post.frag");
        Program hex("Hex\\shaders\\hex.vert", "Hex\\shaders\\hex.frag");

        // Add post-processing to display
        display.addPostProgram(&post);

        // Create instances
        int const gridWidth = 50, gridDepth = 50;
        Pos instances(gridWidth*gridDepth);
        float const xOrigin = -(gridWidth-1)/2.0f, zOrigin = -(gridDepth-1)/2.0f;
        std::cout << xOrigin << ", " << zOrigin << std::endl << std::endl;
        for (int z=0; z<gridDepth; z++)
        {
            for (int x=0; x<gridWidth; x++)
            {
                int const index = (z*gridWidth)+x;

                // Procedural noise
                int seed = 0;
                float const continent = noise::perlinSample(static_cast<float>(x), 0.0f, static_cast<float>(z), 5.0f, seed++)+0.15f;
                float const land = continent;

                // X & Z
                float xVal = (xOrigin+x)*1.5f*HexCell::RADIUS;
                float zVal = (x%2==0) ? (zOrigin+z)*2*HexCell::APOTHEM : (zOrigin+z+0.5f)*2*HexCell::APOTHEM;

                // Y
                float yVal;
                if (land < -0.03f)
                {
                    yVal = 0.0f; // Water
                }
                else if (land < 0.03f)
                {
                    yVal = 0.1f; // Sand
                }
                else if (land < 0.35f)
                {
                    yVal = 0.2f; // Grass
                }
                else
                {
                    yVal = 0.3f; // Rock
                }

                // Gap
                xVal *= 1.00f, zVal *= 1.00f;

                // Set
                instances.position(index)->xyz(xVal, yVal, zVal);
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
