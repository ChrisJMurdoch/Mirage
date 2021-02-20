
#pragma once

namespace noise
{
	float perlinSample(float x, float y, float z, float period);
	float fractal(float x, float y, float z, float period, int octaves);
}
