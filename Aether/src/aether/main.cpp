
#include "utility/geometry.hpp"
#include "utility/objLoader.hpp"

#include "aether/image.hpp"
#include "aether/raytrace.hpp"

#include <glm/gtc/random.hpp>

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator(seed);
std::uniform_real_distribution<double> uniform01(0.0, 1.0);
glm::vec3 randvec()
{
    double theta = 2 * M_PI * uniform01(generator);
    double phi = acos(1 - 2 * uniform01(generator));
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);
    return glm::normalize(glm::vec3(x, y, z));
}

void randomRays(glm::vec3 lightOrigin, Pixel lightAlpha, RayScene const &scene, int nRays)
{
    // Simulate rays
    for (int i=0; i<nRays; i++)
    {
        // Send out a ray in a random direction
        Ray ray{lightOrigin, randvec()};
        std::optional<Hit> hit = scene.getHit(ray);

        // If ray hit
        if (hit)
        {
            // Find intersections point and interpolated UVs
            glm::vec3 intersectionPoint = ray.at(hit->t);
            Vertex interpolatedVertex = hit->triangle->interpolate(intersectionPoint);

            // Get relevant lightmap
            auto &lightmap = hit->triangle->lightmap;
            int x = interpolatedVertex.uv.x*(lightmap.getWidth()-1);
            int y = interpolatedVertex.uv.y*(lightmap.getHeight()-1);

            // Get diffuse gradient
            float diffuseMult = std::clamp( glm::dot(-ray.dir, interpolatedVertex.norm), 0.0f, 1.0f );

            // Modify lightmap pixel
            lightmap[y][x] += lightAlpha*diffuseMult;
        }
    }
}

int run()
{
    std::cout << " // === AETHER === \\\\ " << std::endl << std::endl;

    // Create lightmap targets
    int const res = 500;
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
    int constexpr N_RAYS = 20000000;
    int constexpr N_THREADS = 24;
    float constexpr A = 0.01f;
    Pixel const LIGHT_ALPHA = {A, A, A};
    glm::vec3 constexpr LIGHT_ORIGIN{0.0f, 1.0f, 1.0f};

    auto start = std::chrono::system_clock::now();

    // Simulate rays
    std::vector<std::thread> threads;
    for (int i=0; i<N_THREADS; i++)
        threads.push_back( std::thread(randomRays, LIGHT_ORIGIN, LIGHT_ALPHA, std::ref(scene), N_RAYS/N_THREADS) );
    for (std::thread &thread : threads)
        thread.join();

    // Report timing
    auto end = std::chrono::system_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    float secs = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() / 1000000.0f;
    std::cout << "Rays: " << N_RAYS << " in " << ms << "ms (" << int(N_RAYS/secs) << " rays/s)" << std::endl << std::endl;

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
