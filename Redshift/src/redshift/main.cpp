
#include "display/display.hpp"
#include "shader/program.hpp"
#include "texture/texture.hpp"
#include "model/model.hpp"

#include <vector>
#include <iostream>
#include <exception>
#include <chrono>
#include <math.h>

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
        Program program{"shaders/model.vert", "shaders/model.frag"};

        // Load texture
        Texture texture{"textures/concrete.jpg"};

        // Create model and add to display
        Model square{program, texture, vertices, indices};
        display.registerModel(&square);

        // Render loop
        long long sMicro=0, nFrames=0;
        while(!display.shouldClose())
        {
            // Render
            auto start = std::chrono::system_clock::now();
            display.render();
            auto end = std::chrono::system_clock::now();

            // Calculate and display duration
            nFrames++;
            sMicro += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
            std::cout << "Avg. microseconds: " << sMicro/nFrames << std::endl;
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}