
#include "aether/aether.hpp"

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

void simulateRay(Ray const &ray, RayScene const &scene, glm::vec3 light, int const bounces)
{
    // Test scene for ray collision
    std::optional<Hit> hit = scene.getHit(ray);
    if (!hit)
        return;

    // Find intersection point and interpolated UVs
    glm::vec3 intersectionPoint = ray.at(hit->t);
    Vertex interpolatedVertex = hit->triangle->interpolate(intersectionPoint);

    // Get relevant maps
    auto &lightmap = hit->triangle->material.lightmap;
    int lRow = floor(interpolatedVertex.uv.y*(lightmap->getHeight()));
    int lCol = floor(interpolatedVertex.uv.x*(lightmap->getWidth()));
    auto &normalMap = hit->triangle->material.normal;
    int nRow = floor(interpolatedVertex.uv.y*(normalMap->getHeight()));
    int nCol = floor(interpolatedVertex.uv.x*(normalMap->getWidth()));
    auto &albedoMap = hit->triangle->material.albedo;
    int aRow = floor(interpolatedVertex.uv.y*(albedoMap->getHeight()));
    int aCol = floor(interpolatedVertex.uv.x*(albedoMap->getWidth()));

    // Clamp to avoid border case where UV coord is perfect 1.0
    lRow = std::clamp(lRow, 0, lightmap->getHeight()-1);
    lCol = std::clamp(lCol, 0, lightmap->getWidth()-1);
    nRow = std::clamp(nRow, 0, normalMap->getHeight()-1);
    nCol = std::clamp(nCol, 0, normalMap->getWidth()-1);
    aRow = std::clamp(aRow, 0, albedoMap->getHeight()-1);
    aCol = std::clamp(aCol, 0, albedoMap->getWidth()-1);

    // Calculate TBN
    glm::mat3 tbn = glm::mat3
    {
        glm::normalize(interpolatedVertex.tan),
        glm::normalize(interpolatedVertex.btan),
        glm::normalize(interpolatedVertex.norm)
    };
    glm::vec3 const &tangentSpaceNormal = glm::normalize( static_cast<glm::vec3>((*normalMap)[nRow][nCol]) * 2.0f - 1.0f );
    glm::vec3 finalNormal = tbn * tangentSpaceNormal;

    // Get gradient modifier
    float gradientModifier = std::clamp( glm::dot(-ray.dir, finalNormal), 0.0f, 1.0f );

    // Modify lightmap pixel
    (*lightmap)[lRow][lCol] += light * gradientModifier * hit->triangle->lightModifier;

    // Calculate bounce colour
    glm::vec3 colour = static_cast<glm::vec3>((*albedoMap)[aRow][aCol]);

    // Bounce
    if (bounces>0)
    {
        // Generate new direction - TODO: improve
        glm::vec3 bounceDir;
        do
        {
            bounceDir = randvec();
        } while (glm::dot(finalNormal, bounceDir) <= 0.15f); // TODO: Calculate properly

        // Recursively simulate new ray
        simulateRay( Ray{ray.at(hit->t)+bounceDir*0.001f, bounceDir}, scene, light*colour, bounces-1 );
    }
}

void randomRays(glm::vec3 lightOrigin, float lightRadius, glm::vec3 const light, RayScene const &scene, int nRays, int bounces)
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
        simulateRay(ray, scene, light, bounces);
    }
}

int Aether::run()
{
    std::cout << " // === AETHER === \\\\ " << std::endl << std::endl;

    // Load albedo maps
    Image floorAlbedoMap{"resources/models/cornell/textures/2k/albedo.jpg"};
    Image gargoyleAlbedoMap{"resources/models/gargoyle/textures/2k/albedo.jpg"};

    // Load PBR maps
    Image floorNormalMap{"resources/models/cornell/textures/2k/normal.jpg"};
    Image gargoyleNormalMap{"resources/models/gargoyle/textures/2k/normal.jpg"};

    // Create lightmap targets
    int constexpr res = 1000;
    Image floorLightmap{res, res};
    Image gargoyleLightmap{res, res};

    // Create physical materials
    PhysicalMaterial floorMat{&floorAlbedoMap, &floorNormalMap, &floorLightmap};
    PhysicalMaterial gargoyleMat{&gargoyleAlbedoMap, &gargoyleNormalMap, &gargoyleLightmap};

    // Create raytraceable scene
    std::vector<std::pair<Mesh const, PhysicalMaterial &>> rayMeshes
    {
        { objLoader::loadObj("resources/models/cornell/cornell.obj"), floorMat },
        { objLoader::loadObj("resources/models/gargoyle/gargoyle.obj"), gargoyleMat }
    };
    RayScene scene(rayMeshes);

    // Raytracing parameters
    int constexpr QUALITY = 5; // 1 QUALITY ~= 1s processing
    int constexpr BOUNCES = 2;
    int constexpr N_THREADS = 24;

    // Derivative parameters
    int constexpr T_RAYS = 7500000 * QUALITY;
    int constexpr N_RAYS = T_RAYS / (1+BOUNCES);
    float constexpr A = 0.000006f / QUALITY;
    Pixel constexpr LIGHT_ALPHA{A, A, A};
    glm::vec3 constexpr LIGHT_ORIGIN{0.0f, 1.0f, 1.0f};
    float const LIGHT_RADIUS = 0.75f;

    auto start = std::chrono::system_clock::now();

    // Simulate rays
    {
        std::vector<std::thread> threads;
        for (int i=0; i<N_THREADS-1; i++)
            threads.push_back( std::thread(randomRays, LIGHT_ORIGIN, LIGHT_RADIUS, LIGHT_ALPHA, std::ref(scene), N_RAYS/N_THREADS, BOUNCES) );
        randomRays(LIGHT_ORIGIN, LIGHT_RADIUS, LIGHT_ALPHA, scene, N_RAYS/N_THREADS, BOUNCES);
        for (std::thread& thread : threads)
            thread.join();
    }

    // Report timing
    auto end = std::chrono::system_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    float secs = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() / 1000000.0f;
    std::cout << "Rays: " << T_RAYS << " in " << ms << "ms (" << std::fixed << std::setprecision(2) << (T_RAYS/secs/1000000) << " MegaRays/sec)" << std::endl << std::endl;

    // Save lightmaps to generated/ folders
    floorLightmap.save("resources/models/cornell/generated/lightmap.jpg");
    gargoyleLightmap.save("resources/models/gargoyle/generated/lightmap.jpg");

    std::cout << " \\\\ === AETHER === // " << std::endl;

    return 0;
}
