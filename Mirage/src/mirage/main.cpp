
#include "utility/objLoader.hpp"
#include "utility/fbxLoader.hpp"
#include "utility/geometry.hpp"
#include "render/display.hpp"
#include "render/mtlLoader.hpp"
#include "render/program.hpp"
#include "render/texture.hpp"
#include "render/model.hpp"
#include "aether/aether.hpp"

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

void run()
{
    std::cout << std::endl << " // === MIRAGE === \\\\ " << std::endl << std::endl;

    try
    {
        // Create OpenGL display
        Display display{"Mirage", 1000, 600};

        // Compile shaders into program
        Program program{"resources/shaders/model.vert", "resources/shaders/aether.frag"};

        auto start = std::chrono::system_clock::now();

        // Load floor model
        Mesh floorMesh = objLoader::loadObj("resources/models/cornell/cornell.obj");
        Material floorMaterial = mtlLoader::loadMtl("resources/models/cornell/cornell.mtl");
        Model floor{program, floorMesh, floorMaterial};
        display.registerModel(&floor);

        // Load gargoyle model
        Mesh gargoyleMesh = objLoader::loadObj("resources/models/gargoyle/gargoyle.obj");
        Material gargoyleMaterial = mtlLoader::loadMtl("resources/models/gargoyle/gargoyle.mtl");
        Model gargoyle{program, gargoyleMesh, gargoyleMaterial};
        display.registerModel(&gargoyle);

        // Load FBX models - WIP
        Mesh mesh1 = fbxLoader::loadFbx("resources/models/wall/wall.fbx");
        Mesh mesh2 = fbxLoader::loadFbx("resources/models/statue/statue.fbx");

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
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(display.width)/display.height, 0.1f, 100.0f);

            // Create matrices
            float cHeight = 2, tHeight = 1.5f;
            float const radius = 4.0f;

            glm::vec3 cameraPos = glm::vec3( sin(time())*radius,cHeight, cos(time())*radius );
            //glm::vec3 cameraPos = glm::vec3( sin(0)*radius,cHeight, cos(0)*radius );
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
        
        std::cout << " \\\\ === MIRAGE === // " << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    try
    {
        // Optionally generate lightmaps with aether
        if (argc>=2 && strcmp(argv[1], "aether")==0) Aether::run();

        // Start engine
        run();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
