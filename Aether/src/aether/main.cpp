
#include "utility/geometry.hpp"
#include "utility/objLoader.hpp"

#include "aether/image.hpp"
#include "aether/raytrace.hpp"

#include <glm/gtc/random.hpp>

#include <iostream>
#include <algorithm>
#include <chrono>

int run()
{
    std::cout << " // === AETHER === \\\\ " << std::endl << std::endl;

    // Create lightmap targets
    Image floorLightmap{100, 100};
    Image gargoyleLightmap{100, 100};

    // Create raytraceable scene
    std::vector<raytrace::RayMesh> rayMeshes{
        raytrace::RayMesh{ objLoader::loadObj("resources/models/floor/floor.obj"), floorLightmap },
        raytrace::RayMesh{ objLoader::loadObj("resources/models/gargoyle/gargoyle.obj"), gargoyleLightmap }
    };
    raytrace::RayScene scene(rayMeshes);

    // Raytracing parameters
    int constexpr N_RAYS = 5000;
    Pixel constexpr LIGHT_ALPHA{0.33f, 0.33f, 0.33f};
    glm::vec3 constexpr LIGHT_ORIGIN{0.7f, 0.2f, 0.7f};

    auto start = std::chrono::system_clock::now();

    // Simulate rays
    int hits = 0;
    for (int i=0; i<N_RAYS; i++)
    {
        // Send out a ray in a random direction
        raytrace::Ray ray{LIGHT_ORIGIN, glm::ballRand(1.0f)};
        std::optional<raytrace::MeshHit> meshHit = scene.getHit(ray);

        // If ray hit
        if (meshHit)
        {
            hits++;

            // Find intersections point and interpolated UVs
            glm::vec3 intersectionPoint = ray.at(meshHit->getT());
            Vertex interpolatedVertex = meshHit->triHit.triangle->interpolate(intersectionPoint);

            // Get relevant lightmap
            auto &lightmap = meshHit->mesh->getLightmap();
            int x = interpolatedVertex.uv.x*(lightmap.getWidth()-1);
            int y = interpolatedVertex.uv.y*(lightmap.getHeight()-1);

            // Check bounds and modify lightmap pixel
            if (x>=0 && y>=0 && x<lightmap.getWidth() && y<lightmap.getHeight())
                lightmap[y][x] += LIGHT_ALPHA;
            else
                std::cout << "Out of bounds: " << x << ", " << y << std::endl;
        }
    }

    // Report timing
    auto end = std::chrono::system_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    float secs = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() / 1000000.0f;
    std::cout << "Hits: " << hits << "/" << N_RAYS << " in " << ms << "ms (" << int(N_RAYS/secs) << " rays/s)" << std::endl << std::endl;

    // Save lightmaps to generated/ folders
    floorLightmap.save("resources/models/floor/generated/lightmap.jpg");
    gargoyleLightmap.save("resources/models/gargoyle/generated/lightmap.jpg");

    std::cout << " \\\\ === AETHER === // " << std::endl;

    return 0;
}

int main()
{
    try
    {
        return run();
    }
    catch(std::exception const &e)
    {
        std::cerr << e.what() << '\n';
    }
}
