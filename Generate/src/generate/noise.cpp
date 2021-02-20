
#include "generate/noise.h"

#include <cmath>
#include <stdexcept>
#include <iostream>

/* Interpolate between a and b using x */
inline float lerp(float a, float b, float x)
{
    return a + x * (b - a);
}

/* Smoothing function */
inline float diverge(float x)
{
    const float PI = 3.14159265358979323846;
    return 0.5f - (cos(fmod(x, 1.0f) * PI)*0.5f);
}

/* Combine integer seeds for hashing */
inline int combine(int x, int y) {
    return (x*12345) + y;
}
inline int combine(int x, int y, int z) {
    return combine(combine(x, y), z);
}

/* Quick hashing function */
inline float floatHash(int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return (x % 10000) / 9999.0f;
}

/* Procedurally generate vector from three integers */
inline glm::vec3 getProceduralVector(int X, int Y, int Z)
{
    return glm::normalize( glm::vec3(
        floatHash( combine(X,Y,Z)+0) -0.5,
        floatHash( combine(X,Y,Z)+1) -0.5,
        floatHash( combine(X,Y,Z)+2) -0.5
    ) );
}

float noise::perlinSample(float x, float y, float z, float period)
{
    // Get cube position
    int X = std::floor(x / period);
    int Y = std::floor(y / period);
    int Z = std::floor(z / period);

    // Get sample position relative to cube
    glm::vec3 point = glm::vec3( (x/period)-X, (y/period)-Y, (z/period)-Z );

    // Dot products
    float FBLd = glm::dot( getProceduralVector(X+0, Y+0, Z+0), glm::vec3(0, 0, 0)-point );
    float FBRd = glm::dot( getProceduralVector(X+1, Y+0, Z+0), glm::vec3(1, 0, 0)-point );
    float FTLd = glm::dot( getProceduralVector(X+0, Y+1, Z+0), glm::vec3(0, 1, 0)-point );
    float FTRd = glm::dot( getProceduralVector(X+1, Y+1, Z+0), glm::vec3(1, 1, 0)-point );
    float CBLd = glm::dot( getProceduralVector(X+0, Y+0, Z+1), glm::vec3(0, 0, 1)-point );
    float CBRd = glm::dot( getProceduralVector(X+1, Y+0, Z+1), glm::vec3(1, 0, 1)-point );
    float CTLd = glm::dot( getProceduralVector(X+0, Y+1, Z+1), glm::vec3(0, 1, 1)-point );
    float CTRd = glm::dot( getProceduralVector(X+1, Y+1, Z+1), glm::vec3(1, 1, 1)-point );

    // Interpolate using diverge
    float value = lerp(
        lerp(
            lerp(FBLd, FBRd, diverge(point.x)),
            lerp(FTLd, FTRd, diverge(point.x)),
            diverge(point.y)
        ),
        lerp(
            lerp(CBLd, CBRd, diverge(point.x)),
            lerp(CTLd, CTRd, diverge(point.x)),
            diverge(point.y)
        ),
        diverge(point.z)
    );

    // Move value into 0-1 range
    return (value+1) / 2;
}

float noise::fractalSample(float x, float y, float z, float period, int octaves)
{
    // Settings
    static const float LACUNARITY = 0.5, PERSISTANCE = 0.5;

    // Octaves
    float value = 0, max = 0;
    for (int o=0; o<octaves; o++)
    {
        // Caluculate amplitude and period
        float pmult = pow(LACUNARITY, o), amplitude = pow(PERSISTANCE, o);

        // Calculate value
        value += amplitude * perlinSample(x, y, z, pmult*period);
        max   += amplitude * 1;
    }
    return value / max;
}
