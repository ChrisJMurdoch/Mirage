
#include "display/display.hpp"
#include "shader/program.hpp"
#include "model/model.hpp"

#include <vector>
#include <iostream>
#include <exception>
#include <chrono>
#include <math.h>

std::vector<Vertex> vertices
{
    Vertex{ Vec3{ 0.5f, -0.5f, 0.0f},  Vec3{1.0f, 0.0f, 0.0f} },
    Vertex{ Vec3{-0.5f, -0.5f, 0.0f},  Vec3{0.0f, 1.0f, 0.0f} },
    Vertex{ Vec3{ 0.0f,  0.5f, 0.0f},  Vec3{0.0f, 0.0f, 1.0f} }
};
std::vector<unsigned int> indices
{
    0, 1, 2,
};

int main()
{
    try
    {
        // Create OpenGL display
        Display display{"Redshift", 1000, 600};

        // Compile shaders into program
        Program program{"shaders/model.vert", "shaders/model.frag"};

        // Create model and add to display
        Model square{program, vertices, indices};
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
