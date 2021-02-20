
#include "generate/noise.h"

#include <glm/glm.hpp>

#include <cmath>

float lerp(float a, float b, float x)
{
    return a + x * (b - a);
}

float diverge(float x)
{
    const float PI = 3.14159265358979323846;
    return 0.5f - (cos(fmod(x, 1.0f) * PI)*0.5f);
}

int combine(int x, int y) {
    return (x*12345) + y;
}
int combine(int x, int y, int z) {
    return combine(combine(x, y), z);
}

float floatHash(int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return (x % 10000) / 9999.0f;
}

float noise::perlinSample(float x, float y, float z, float period)
{
    // Square coords
    int X = std::floor(x / period);
    int Y = std::floor(y / period);
    int Z = std::floor(z / period);

    // Relative point coords
    float rx = (x/period) - X;
    float ry = (y/period) - Y;
    float rz = (z/period) - Z;

    // Square corner vectors
    glm::vec3 FBL = glm::normalize(glm::vec3(floatHash(combine(X+0, Y+0, Z+0))-0.5, floatHash(combine(X+0, Y+0, Z+0)+1)-0.5, floatHash(combine(X+0, Y+0, Z+0)+2)-0.5));
    glm::vec3 FBR = glm::normalize(glm::vec3(floatHash(combine(X+1, Y+0, Z+0))-0.5, floatHash(combine(X+1, Y+0, Z+0)+1)-0.5, floatHash(combine(X+1, Y+0, Z+0)+2)-0.5));
    glm::vec3 FTL = glm::normalize(glm::vec3(floatHash(combine(X+0, Y+1, Z+0))-0.5, floatHash(combine(X+0, Y+1, Z+0)+1)-0.5, floatHash(combine(X+0, Y+1, Z+0)+2)-0.5));
    glm::vec3 FTR = glm::normalize(glm::vec3(floatHash(combine(X+1, Y+1, Z+0))-0.5, floatHash(combine(X+1, Y+1, Z+0)+1)-0.5, floatHash(combine(X+1, Y+1, Z+0)+2)-0.5));
    glm::vec3 CBL = glm::normalize(glm::vec3(floatHash(combine(X+0, Y+0, Z+1))-0.5, floatHash(combine(X+0, Y+0, Z+1)+1)-0.5, floatHash(combine(X+0, Y+0, Z+1)+2)-0.5));
    glm::vec3 CBR = glm::normalize(glm::vec3(floatHash(combine(X+1, Y+0, Z+1))-0.5, floatHash(combine(X+1, Y+0, Z+1)+1)-0.5, floatHash(combine(X+1, Y+0, Z+1)+2)-0.5));
    glm::vec3 CTL = glm::normalize(glm::vec3(floatHash(combine(X+0, Y+1, Z+1))-0.5, floatHash(combine(X+0, Y+1, Z+1)+1)-0.5, floatHash(combine(X+0, Y+1, Z+1)+2)-0.5));
    glm::vec3 CTR = glm::normalize(glm::vec3(floatHash(combine(X+1, Y+1, Z+1))-0.5, floatHash(combine(X+1, Y+1, Z+1)+1)-0.5, floatHash(combine(X+1, Y+1, Z+1)+2)-0.5));

    // Relational vectors
    glm::vec3 point = glm::vec3(rx, ry, rz);
    glm::vec3 FBLr = glm::vec3(0, 0, 0) - point;
    glm::vec3 FBRr = glm::vec3(1, 0, 0) - point;
    glm::vec3 FTLr = glm::vec3(0, 1, 0) - point;
    glm::vec3 FTRr = glm::vec3(1, 1, 0) - point;
    glm::vec3 CBLr = glm::vec3(0, 0, 1) - point;
    glm::vec3 CBRr = glm::vec3(1, 0, 1) - point;
    glm::vec3 CTLr = glm::vec3(0, 1, 1) - point;
    glm::vec3 CTRr = glm::vec3(1, 1, 1) - point;

    // Dot products
    float FBLd = glm::dot(FBL, FBLr);
    float FBRd = glm::dot(FBR, FBRr);
    float FTLd = glm::dot(FTL, FTLr);
    float FTRd = glm::dot(FTR, FTRr);
    float CBLd = glm::dot(CBL, CBLr);
    float CBRd = glm::dot(CBR, CBRr);
    float CTLd = glm::dot(CTL, CTLr);
    float CTRd = glm::dot(CTR, CTRr);

    // Interpolate using diverge
    float Fbottom = lerp(FBLd, FBRd, diverge(point.x));
    float Ftop = lerp(FTLd, FTRd, diverge(point.x));
    float Fcentre = lerp(Fbottom, Ftop, diverge(point.y));
    float Cbottom = lerp(CBLd, CBRd, diverge(point.x));
    float Ctop = lerp(CTLd, CTRd, diverge(point.x));
    float Ccentre = lerp(Cbottom, Ctop, diverge(point.y));
    float last = lerp(Fcentre, Ccentre, diverge(point.z));

    // 0-1
    return (last+1) / 2;
}

float noise::fractal(float x, float y, float z, float period, int octaves)
{
    // Octaves
    float height = 0;
    float max = 0;
    for (int o=0; o<octaves; o++)
    {
        // Caluculate amplitude and period
        const float lacunarity = 0.5, persistance = 0.4;
        float pmult = pow(lacunarity, o), amplitude = pow(persistance, o);

        height += perlinSample(x, y, z, pmult*period) * amplitude;

        max += amplitude;
    }
    return height / max;
}
