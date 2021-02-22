
#pragma once

#include "model/vertexArray.h"

#include <glm/glm.hpp>

namespace noise
{
	float perlinSample(float x, float y, float z, float period);
	float fractalSample(float x, float y, float z, float period, int octaves);
}
