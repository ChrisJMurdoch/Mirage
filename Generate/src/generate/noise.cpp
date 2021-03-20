
#include "generate/noise.hpp"

#include "utility/math.hpp"

#include <math.h>
#include <cmath>
#include <algorithm>

/* Procedurally generate vector from three integers */
inline glm::vec3 getProceduralVector(int X, int Y, int Z, int seed)
{
    return glm::normalize( glm::vec3(
        math::floatHash( math::combine(X,Y,Z,seed+0) ) -0.5,
        math::floatHash( math::combine(X,Y,Z,seed+1) ) -0.5,
        math::floatHash( math::combine(X,Y,Z,seed+2) ) -0.5
    ) );
}

float noise::perlinSample(float x, float y, float z, float period, int seed)
{
    // Get cube position
    int X = (int)std::floor(x / period);
    int Y = (int)std::floor(y / period);
    int Z = (int)std::floor(z / period);

    // Get sample position relative to cube
    glm::vec3 point = glm::vec3( (x/period)-X, (y/period)-Y, (z/period)-Z );

    // Dot products
    float FBLd = glm::dot( getProceduralVector(X+0, Y+0, Z+0, seed), glm::vec3(0, 0, 0)-point );
    float FBRd = glm::dot( getProceduralVector(X+1, Y+0, Z+0, seed), glm::vec3(1, 0, 0)-point );
    float FTLd = glm::dot( getProceduralVector(X+0, Y+1, Z+0, seed), glm::vec3(0, 1, 0)-point );
    float FTRd = glm::dot( getProceduralVector(X+1, Y+1, Z+0, seed), glm::vec3(1, 1, 0)-point );
    float CBLd = glm::dot( getProceduralVector(X+0, Y+0, Z+1, seed), glm::vec3(0, 0, 1)-point );
    float CBRd = glm::dot( getProceduralVector(X+1, Y+0, Z+1, seed), glm::vec3(1, 0, 1)-point );
    float CTLd = glm::dot( getProceduralVector(X+0, Y+1, Z+1, seed), glm::vec3(0, 1, 1)-point );
    float CTRd = glm::dot( getProceduralVector(X+1, Y+1, Z+1, seed), glm::vec3(1, 1, 1)-point );

    // Interpolate using diverge
    float value = math::lerp(
        math::lerp(
            math::lerp(FBLd, FBRd, math::smooth(point.x)),
            math::lerp(FTLd, FTRd, math::smooth(point.x)),
            math::smooth(point.y)
        ),
        math::lerp(
            math::lerp(CBLd, CBRd, math::smooth(point.x)),
            math::lerp(CTLd, CTRd, math::smooth(point.x)),
            math::smooth(point.y)
        ),
        math::smooth(point.z)
    );

    return value;
}

float noise::fractalSample(float x, float y, float z, float period, int octaves, int seed)
{
    // Settings
    static const float LACUNARITY = 0.5f, PERSISTENCE = 0.52f;

    // Octaves
    float value = 0.0f, max = 0.0f;
    for (int o=0; o<octaves; o++)
    {
        // Caluculate amplitude and period
        float const pmult = (float)pow(LACUNARITY, o), amplitude = (float)pow(PERSISTENCE, o);

        // Calculate value
        value += amplitude * perlinSample(x, y, z, pmult*period, seed);
        max   += amplitude * 1.0f;
    }
    return value / max;
}
