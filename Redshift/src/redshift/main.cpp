
#include "display/display.hpp"
#include "display/objLoader.hpp"
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

int main()
{
    try
    {
        // Create OpenGL display
        Display display{"Redshift", 1000, 600};

        // Compile shaders into program
        Program program{"resources/shaders/model.vert", "resources/shaders/model.frag"};

        // Load geometry
        std::pair<std::vector<Vertex>, std::vector<unsigned int>> objData = objLoader::loadObj("resources/models/gargoyle/gargoyle.obj", true);

        // Load texture
        Texture albedo{"resources/models/gargoyle/textures/2k/albedo.jpg"};
        Texture normal{"resources/models/gargoyle/textures/2k/normal.jpg"};
        Texture roughness{"resources/models/gargoyle/textures/2k/roughness.jpg"};

        // Create model and add to display
        Model square{program, objData.first, objData.second, &albedo, &normal, &roughness};
        display.registerModel(&square);

        // Render loop
        float t = 0;
        while(!display.shouldClose())
        {
            // Create matrices
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(t+=1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.8f));
            glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.95f, -3.0f));
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
