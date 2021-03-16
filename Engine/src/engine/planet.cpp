
#include "engine/planet.h"

#include "utility/math.h"
#include "generate/noise.h"

#include <math.h>
#include <iostream>

// DOMAIN WARPING
static float const WARP_MAGNITUDE = 0.15f, WARP_PERIOD = 0.25f;

// NOISE
static float const MAGNITUDE = 0.5f;
static float const LAND_MAGNITUDE = 0.5f, LAND_PERIOD = 0.5f;
static float const FINE_MAGNITUDE = 0.005f, FINE_PERIOD = 0.01f;

// MOUNTAIN MASK
static float const MMASK_PERIOD = 0.25f, MMASK_BIAS = 1.00f;
static int const MMASK_OCTAVES = 2, MMASK_PASSES = 3;

Planet::Planet(int octaves) : octaves(octaves) {}

static inline float ridge(float input)
{
    return 1 - abs(input)*2; // (-1,1) [mode=0] -> (-1,1) [mode=1]
}

static inline float createMask(float noise, int smoothPasses, float bias)
{
    float mask = (noise+1)/2; // (-1,1) [mode=0] -> (0,1) [mode=0.5]
    mask *= bias;
    for (int i=0; i<smoothPasses; i++)
        mask = math::smooth(mask); // Standard deviation ++
    return mask;
}

void Planet::setPosition(VirtualVector position)
{
    // Create sphere for sample coords
    position.normalise();

    // Get vector components
    float x=position.getX(), y=position.getY(), z=position.getZ();

    // Seed hashes
    int seed = 0;

    // Domain warp
    x += WARP_MAGNITUDE * noise::perlinSample(x, y, z, WARP_PERIOD, seed++);
    y += WARP_MAGNITUDE * noise::perlinSample(x, y, z, WARP_PERIOD, seed++);
    z += WARP_MAGNITUDE * noise::perlinSample(x, y, z, WARP_PERIOD, seed++);

    // LAND SHAPE

    // Generate noise values
    float land_noise = noise::fractalSample(x, y, z, LAND_PERIOD, octaves, seed++);
    float fine_noise = noise::fractalSample(x, y, z, FINE_PERIOD, 3, seed++);

    // Generate noise masks
    float mmask = createMask(noise::fractalSample(x, y, z, MMASK_PERIOD, MMASK_OCTAVES, seed++), MMASK_PASSES, MMASK_BIAS);

    // Mix values
    float value = (mmask*land_noise*LAND_MAGNITUDE) + (fine_noise*FINE_MAGNITUDE);

    // Raise land
    value += 0.01f;

    // Set height
    float height = 1 + value*MAGNITUDE;
    position.normalise(height);
}

void Planet::setColour(VirtualVector position, VirtualVector normal, VirtualVector colour)
{
    glm::vec3 position_vec = glm::normalize( position.asVec3() );
    glm::vec3 normal_vec = glm::normalize( normal.asVec3() );

    float flatness = glm::dot(position_vec, normal_vec);
    flatness = pow(flatness, 24);

    glm::vec3 const SAND(0.9f, 0.9f, 0.7f);
    glm::vec3 const GRASS(0.3f, 0.6f, 0.1f);
    glm::vec3 const STONE(0.3f, 0.3f, 0.3f);
    glm::vec3 const SNOW(1.0f, 1.0f, 1.0f);

    float const height = position.getMagnitude();
    glm::vec3 colour_vec;
    if (height<1.004f)
        colour_vec = SAND;
    else if (height<1.02f)
        colour_vec = (GRASS*flatness) + (STONE*(1-flatness));
    else if (height<1.03f)
        colour_vec = STONE;
    else
        colour_vec = flatness>0.5f ? SNOW : STONE;

    colour.set(colour_vec.x, colour_vec.y, colour_vec.z);
}
