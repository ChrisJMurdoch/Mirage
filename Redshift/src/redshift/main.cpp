
#include "display/display.hpp"
#include "display/geometry.hpp"
#include "display/program.hpp"
#include "display/texture.hpp"
#include "display/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <exception>
#include <chrono>

std::vector<Vertex> vertices
{
    // Close quad
    Vertex{ Vec3{-1.0f, -1.0f, -1.0f},  Vec2{-1.0f, -1.0f} },   // Top left
    Vertex{ Vec3{ 1.0f, -1.0f, -1.0f},  Vec2{1.0f, -1.0f} },    // Top right
    Vertex{ Vec3{ 1.0f,  1.0f, -1.0f},  Vec2{1.0f, 1.0f} },     // Bottom right
    Vertex{ Vec3{-1.0f,  1.0f, -1.0f},  Vec2{-1.0f, 1.0f} },    // Bottom left

    // Far quad
    Vertex{ Vec3{-1.0f, -1.0f,  1.0f},  Vec2{-1.0f, -1.0f} },   // Top left
    Vertex{ Vec3{ 1.0f, -1.0f,  1.0f},  Vec2{1.0f, -1.0f} },    // Top right
    Vertex{ Vec3{ 1.0f,  1.0f,  1.0f},  Vec2{1.0f, 1.0f} },     // Bottom right
    Vertex{ Vec3{-1.0f,  1.0f,  1.0f},  Vec2{-1.0f, 1.0f} },    // Bottom left
};
std::vector<unsigned int> indices
{
    // Close
    0, 1, 2,
    2, 3, 0,

    // Far
    4, 5, 6,
    6, 7, 4,
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
        {
            // Create matrices
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            // Send to GPU
            program.setUniformMat4("model", model);
            program.setUniformMat4("view", view);
            program.setUniformMat4("projection", projection);

            // Refresh display
            display.render();
        }
        
        std::cout << "Application shutting down gracefully." << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
