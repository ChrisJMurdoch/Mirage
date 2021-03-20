
#pragma once

#include "model/vertexArray.hpp"

#include <glm/glm.hpp>

namespace noise
{
	float perlinSample(float x, float y, float z, float period, int seed=0);
	float fractalSample(float x, float y, float z, float period, int octaves, int seed=0);
}
