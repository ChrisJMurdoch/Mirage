
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

thread_local std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_real_distribution<double> uniform01(0.0, 1.0);

// From http://corysimon.github.io/articles/uniformdistn-on-sphere/
glm::vec3 randvec()
{
    double theta = 2 * M_PI * uniform01(generator);
    double phi = acos(1 - 2 * uniform01(generator));
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);
    return glm::normalize(glm::vec3(x, y, z));
}

void randomRays(glm::vec3 lightOrigin, float lightRadius, Pixel const lightAlpha, RayScene const &scene, int nRays)
{
    // Simulate rays
    for (int i=0; i<nRays; i++)
    {
        // Send out a ray in a random direction
        Ray ray
        {
            lightOrigin + randvec()*lightRadius*static_cast<float>(uniform01(generator)), // TODO: Improve sphere distribution
            randvec()
        };
        std::optional<Hit> hit = scene.getHit(ray);

        // If ray hit
        if (hit)
        {
            // Find intersection point and interpolated UVs
            glm::vec3 intersectionPoint = ray.at(hit->t);
            Vertex interpolatedVertex = hit->triangle->interpolate(intersectionPoint);

            // Get relevant maps
            auto &lightmap = hit->triangle->material.lightmap;
            int lRow = interpolatedVertex.uv.y*(lightmap->getHeight()-1);
            int lCol = interpolatedVertex.uv.x*(lightmap->getWidth()-1);
            auto &normalMap = hit->triangle->material.normal;
            int nRow = interpolatedVertex.uv.y*(normalMap->getHeight()-1);
            int nCol = interpolatedVertex.uv.x*(normalMap->getWidth()-1);

            // Calculate TBN
            glm::mat3 tbn = glm::mat3
            {
                glm::normalize(interpolatedVertex.tan),
                glm::normalize(interpolatedVertex.btan),
                glm::normalize(interpolatedVertex.norm)
            };
            glm::vec3 const &tangentSpaceNormal = glm::normalize( static_cast<glm::vec3>((*normalMap)[nRow][nCol]) * 2.0f - 1.0f );
            glm::vec3 finalNormal = tbn * tangentSpaceNormal;

            // Get diffuse gradient
            float diffuseMult = std::clamp( glm::dot(-ray.dir, finalNormal), 0.0f, 1.0f );

            // Modify lightmap pixel
            (*lightmap)[lRow][lCol] += lightAlpha * diffuseMult;
        }
    }
}

int run()
{
    std::cout << " // === AETHER === \\\\ " << std::endl << std::endl;

    // Load PBR maps
    Image floorNormalMap{"resources/models/cornell/textures/2k/normal.jpg"};
    Image gargoyleNormalMap{"resources/models/gargoyle/textures/2k/normal.jpg"};

    // Create lightmap targets
    int constexpr res = 1000;
    Image floorLightmap{res, res};
    Image gargoyleLightmap{res, res};

    // Create physical materials
    PhysicalMaterial floorMat{&floorNormalMap, &floorLightmap};
    PhysicalMaterial gargoyleMat{&gargoyleNormalMap, &gargoyleLightmap};

    // Create raytraceable scene
    std::vector<std::pair<Mesh const, PhysicalMaterial &>> rayMeshes
    {
        { objLoader::loadObj("resources/models/cornell/cornell.obj"), floorMat },
        { objLoader::loadObj("resources/models/gargoyle/gargoyle.obj"), gargoyleMat }
    };
    RayScene scene(rayMeshes);

    // Raytracing parameters
    int constexpr QUALITY = 30; // 1 QUALITY ~= 1s processing
    int constexpr N_RAYS = 10000000 * QUALITY;
    int constexpr N_THREADS = 24;
    float constexpr A = 0.05f / QUALITY;
    Pixel constexpr LIGHT_ALPHA{A, A, A};
    glm::vec3 constexpr LIGHT_ORIGIN{0.0f, 1.0f, 1.0f};
    float const LIGHT_RADIUS = 0.0f;

    auto start = std::chrono::system_clock::now();

    // Simulate rays
    std::vector<std::thread> threads;
    for (int i=0; i<N_THREADS; i++)
        threads.push_back( std::thread(randomRays, LIGHT_ORIGIN, LIGHT_RADIUS, LIGHT_ALPHA, std::ref(scene), N_RAYS/N_THREADS) );
    for (std::thread &thread : threads)
        thread.join();

    // Report timing
    auto end = std::chrono::system_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    float secs = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() / 1000000.0f;
    std::cout << "Rays: " << N_RAYS << " in " << ms << "ms (" << int(N_RAYS/secs/1000000) << " MegaRays/sec)" << std::endl << std::endl;

    // Save lightmaps to generated/ folders
    floorLightmap.save("resources/models/cornell/generated/lightmap.jpg");
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
