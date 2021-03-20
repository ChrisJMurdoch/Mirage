
#pragma once

#include <glm/glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <algorithm>

namespace math
{
    /* Interpolate between a and b using x */
    inline float lerp(float a, float b, float x)
    {
        return a + x * (b - a);
    }

    /* Smoothing function */
    inline float smooth(float x)
    {
        return 0.5f - ((float)cos(fmod(x, 1.0f) * M_PI)*0.5f);
    }

    /* Combine integer seeds for hashing */
    inline int combine(int x, int y) {
        return (x*12345) + y;
    }
    inline int combine(int x, int y, int z) {
        return combine(combine(x, y), z);
    }
    inline int combine(int x, int y, int z, int w) {
        return combine(combine(x, y, z), w);
    }

    /* Quick hashing function */
    inline float floatHash(int x)
    {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return (x % 10000) / 9999.0f;
    }
}
