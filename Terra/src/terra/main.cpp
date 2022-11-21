
#include "utility/objLoader.hpp"
#include "utility/geometry.hpp"

#include "render/display.hpp"
#include "render/mtlLoader.hpp"
#include "render/program.hpp"
#include "render/texture.hpp"
#include "render/model.hpp"

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
    std::cout << " // === TERRA === \\\\ " << std::endl << std::endl;

    try
    {
        // Create OpenGL display
        Display display{"Redshift", 1000, 600};

        // Compile shaders into program
        Program program{"resources/shaders/model.vert", "resources/shaders/model.frag"};

        auto start = std::chrono::system_clock::now();

        // Generate and register land model
        Mesh floorMesh{
            std::vector<Vertex>{
                Vertex{glm::vec3{-1, 1,-1}, glm::vec2{0,0}, glm::vec3{-1, 1,-1}},
                Vertex{glm::vec3{ 1, 0,-1}, glm::vec2{1,0}, glm::vec3{ 1, 1,-1}},
                Vertex{glm::vec3{ 1, 1, 1}, glm::vec2{1,1}, glm::vec3{ 1, 1, 1}},
                Vertex{glm::vec3{-1, 0, 1}, glm::vec2{0,1}, glm::vec3{-1, 1, 1}}
            },
            std::vector<unsigned int>
            {
                0, 1, 2,
                2, 3, 0
            }
        };
        objLoader::generateTangents(floorMesh.indices, floorMesh.vertices);
        Material floorMaterial = mtlLoader::loadMtl("resources/models/floor/floor.mtl");
        Model floor{program, floorMesh, floorMaterial};
        display.registerModel(&floor);

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
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            // Create matrices
            float cHeight = 2, tHeight = 1;
            float const radius = 3.0f;
            
            glm::vec3 cameraPos = glm::vec3( sin(time())*radius,cHeight, cos(time())*radius );
            glm::vec3 cameraTarget = glm::vec3(0.0f, tHeight, 0.0f);
            glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
            glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);

            // Send to GPU
            program.setUniformMat4("model", model);
            program.setUniformMat4("view", view);
            program.setUniformMat4("projection", projection);

            // Refresh display
            display.render();
        }
        
        std::cout << " \\\\ === TERRA === // " << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
