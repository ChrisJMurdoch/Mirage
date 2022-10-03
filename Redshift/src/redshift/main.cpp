
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

float time()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000000000.0f;
}

int main()
{

    std::cout << " === ENGINE STARTING === " << std::endl << std::endl;

    try
    {
        // Create OpenGL display
        Display display{"Redshift", 1000, 600};

        // Compile shaders into program
        Program program{"resources/shaders/model.vert", "resources/shaders/model.frag"};

        // Load floor model
        std::pair<std::vector<Vertex>, std::vector<unsigned int>> floorMesh = objLoader::loadObj("resources/models/floor/floor.obj", true);
        Texture floorAlbedo{"resources/models/floor/textures/2k/albedo.jpg", true};
        Texture floorNormal{"resources/models/floor/textures/2k/normal.jpg", true};
        Texture floorRoughness{"resources/models/floor/textures/2k/roughness.jpg", true};
        Model floor{program, floorMesh.first, floorMesh.second, &floorAlbedo, &floorNormal, &floorRoughness};
        display.registerModel(&floor);

        // Load gargoyle model
        std::pair<std::vector<Vertex>, std::vector<unsigned int>> gargoyleMesh = objLoader::loadObj("resources/models/gargoyle/gargoyle.obj", true);
        Texture gargoyleAlbedo{"resources/models/gargoyle/textures/2k/albedo.jpg", true};
        Texture gargoyleNormal{"resources/models/gargoyle/textures/2k/normal.jpg", true};
        Texture gargoyleRoughness{"resources/models/gargoyle/textures/2k/roughness.jpg", true};
        Model gargoyle{program, gargoyleMesh.first, gargoyleMesh.second, &gargoyleAlbedo, &gargoyleNormal, &gargoyleRoughness};
        display.registerModel(&gargoyle);

        std::cout << std::endl;

        // Render loop
        auto lastTick = std::chrono::system_clock::now();
        while(!display.shouldClose())
        {
            // Seconds since last frame start
            float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastTick).count() / 1000000.0f;

            // Create matrices
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime*60), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.2f));
            glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -3.0f));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            // Send to GPU
            program.setUniformMat4("model", model);
            program.setUniformMat4("view", view);
            program.setUniformMat4("projection", projection);

            // Refresh display
            display.render();
        }
        
        std::cout << " === ENGINE DOWN === " << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
