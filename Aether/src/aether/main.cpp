
#include "utility/geometry.hpp"
#include "utility/objLoader.hpp"

#include "aether/image.hpp"
#include "aether/rayGeometry.hpp"

#include <glm/gtc/random.hpp>

#include <iostream>
#include <algorithm>
#include <chrono>

int run()
{
    std::cout << " // === AETHER === \\\\ " << std::endl << std::endl;

    // Create lightmap targets
    int const res = 300;
    Image floorLightmap{res, res};
    Image gargoyleLightmap{res, res};

    // Create raytraceable scene
    std::vector<std::pair<Mesh const, Image &>> rayMeshes
    {
        { objLoader::loadObj("resources/models/floor/floor.obj"), floorLightmap },
        { objLoader::loadObj("resources/models/gargoyle/gargoyle.obj"), gargoyleLightmap }
    };
    RayScene scene(rayMeshes);

    // Raytracing parameters
    int constexpr N_RAYS = 100000;
    float constexpr A = 1.0f;
    Pixel constexpr LIGHT_ALPHA{A, A, A};
    glm::vec3 constexpr LIGHT_ORIGIN{0.01f, 0.75f, 0.75f};

    auto start = std::chrono::system_clock::now();

    // Simulate rays
    int hits = 0;
    for (int i=0; i<N_RAYS; i++)
    {
        // Send out a ray in a random direction
        Ray ray{LIGHT_ORIGIN, glm::normalize(glm::ballRand(1.0f))};
        std::optional<Hit> triHit = scene.getHit(ray);

        // If ray hit
        if (triHit)
        {
            hits++;

            // Find intersections point and interpolated UVs
            glm::vec3 intersectionPoint = ray.at(triHit->getT());
            Vertex interpolatedVertex = triHit->triangle->interpolate(intersectionPoint);

            // Get relevant lightmap
            auto &lightmap = triHit->triangle->lightmap;
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
