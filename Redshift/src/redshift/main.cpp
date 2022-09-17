
#include "display/display.hpp"
#include "display/geometry.hpp"
#include "display/program.hpp"
#include "display/texture.hpp"
#include "display/model.hpp"

#include <vector>
#include <iostream>
#include <exception>
#include <chrono>

std::vector<Vertex> vertices
{
    Vertex{ Vec3{-0.9f, -0.9f, 0.0f},  Vec2{0.0f, 0.0f} },
    Vertex{ Vec3{ 0.9f, -0.9f, 0.0f},  Vec2{1.0f, 0.0f} },
    Vertex{ Vec3{ 0.9f,  0.9f, 0.0f},  Vec2{1.0f, 1.0f} },
    Vertex{ Vec3{-0.9f,  0.9f, 0.0f},  Vec2{0.0f, 1.0f} }
};
std::vector<unsigned int> indices
{
    0, 1, 2,
    2, 3, 0,
};

int main()
{
    try
    {
        // Create OpenGL display
        Display display{"Redshift", 1000, 600};

        // Compile shaders into program
        Program program{"resources/shaders/model.vert", "resources/shaders/model.frag"};

        // Load texture
        Texture texture{"resources/textures/concrete.jpg"};

        // Create model and add to display
        Model square{program, texture, vertices, indices};
        display.registerModel(&square);

        // Render loop
        while(!display.shouldClose())
            display.render();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
