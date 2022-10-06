
#include "display/display.hpp"
#include "display/objLoader.hpp"
#include "display/mtlLoader.hpp"
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
    static std::chrono::time_point start = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() / 1000000.0f;
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

        auto start = std::chrono::system_clock::now();

        // Load floor model
        Mesh floorMesh = objLoader::loadObj("resources/models/floor/floor.obj");
        Material floorMaterial = mtlLoader::loadMtl("resources/models/floor/floor.mtl");
        Model floor{program, floorMesh, floorMaterial};
        display.registerModel(&floor);

        // Load gargoyle model
        Mesh gargoyleMesh = objLoader::loadObj("resources/models/gargoyle/gargoyle.obj");
        Material gargoyleMaterial = mtlLoader::loadMtl("resources/models/gargoyle/gargoyle.mtl");
        Model gargoyle{program, gargoyleMesh, gargoyleMaterial};
        display.registerModel(&gargoyle);

        auto end = std::chrono::system_clock::now();
        std::cout << "Loaded models (" << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms)" << std::endl;

        std::cout << std::endl;

        // Render loop
        auto lastTick = std::chrono::system_clock::now();
        while(!display.shouldClose())
        {
            // Seconds since last frame start
            float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastTick).count() / 1000000.0f;

            // Create matrices
            float height = 1.0f;
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            // Create matrices
            float const radius = 3.0f;
            glm::vec3 cameraPos = glm::vec3( sin(time())*radius,height, cos(time())*radius );
            glm::vec3 cameraTarget = glm::vec3(0.0f, height, 0.0f);
            glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
            glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);

            // Send to GPU
            program.setUniformMat4("model", model);
            program.setUniformMat4("view", view);
            program.setUniformMat4("projection", projection);

            // Refresh display
            display.render();
        }
        
        std::cout << " === ENGINE SHUTTING DOWN === " << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
