
#include "terra/generate.hpp"

#include "utility/geometry.hpp"

uint32_t intHash(uint32_t a)
{
	a = (a+0x479ab41d) + (a<<8);
	a = (a^0xe4aa10ce) ^ (a>>5);
	a = (a+0x9942f0a6) - (a<<14);
	a = (a^0x5aedd67d) ^ (a>>3);
	a = (a+0x17bea992) + (a<<7);
	return a;
}

glm::vec2 randomVector(int16_t X, int16_t Y, int32_t seed)
{
    // Shift signed ints into positive range
    uint16_t x=(1<<15)+X, y=(1<<15)+Y;

    // Calculate hash of combined coordinates
    int32_t hashedSeed = intHash(seed);
    uint32_t combinedCoordinates = (static_cast<uint32_t>(x)<<16) | (y);
    int32_t hash1=intHash(combinedCoordinates^hashedSeed),
            hash2=intHash(combinedCoordinates^hashedSeed^0x11111111);

    // Generate unit vector
    return glm::normalize(glm::vec2{hash1, hash2});
}

float smootherstep(float t)
{
	return 6*std::pow(t, 5) - 15*std::pow(t, 4) + 10*std::pow(t, 3);
}

float lerp(float a, float b, float t)
{
	return a + (b-a)*t;
}

float perlinNoise(float x, float y, int seed)
{
	// Grid cell coordinates
	int16_t X=std::floor(x), Y=std::floor(y);

	// Sample coordinates within cell
	glm::vec2 sample{x-X, y-Y};

	// Get sample vectors relative to each corner of the grid cell
	glm::vec2 rBL = sample - glm::vec2{0, 0};
	glm::vec2 rBR = sample - glm::vec2{1, 0};
	glm::vec2 rTL = sample - glm::vec2{0, 1};
	glm::vec2 rTR = sample - glm::vec2{1, 1};

	// Generate pseudorandom vectors for each corner of the cell
	glm::vec2 pBL = randomVector(X,   Y,   seed);
	glm::vec2 pBR = randomVector(X+1, Y,   seed);
	glm::vec2 pTL = randomVector(X,   Y+1, seed);
	glm::vec2 pTR = randomVector(X+1, Y+1, seed);

	// Calculate dot products for each relative and pseudorandom vector
	float dBL = glm::dot(pBL, rBL);
	float dBR = glm::dot(pBR, rBR);
	float dTL = glm::dot(pTL, rTL);
	float dTR = glm::dot(pTR, rTR);

	// Interpolate between corner dot products using smootherstep
	float xLerp=smootherstep(sample.x), yLerp=smootherstep(sample.y);
	float dB = lerp(dBL, dBR, xLerp);
	float dT = lerp(dTL, dTR, xLerp);
	float d  = lerp(dB,  dT,  yLerp);

	return d;
}

Mesh generate::generateMesh(unsigned int dimension, float horizontalScale)
{
    std::vector<Vertex> vertices;
    for (int z=0; z<dimension; z++)
    {
        for (int x=0; x<dimension; x++)
        {
            float fz = (static_cast<float>(z)/dimension * 2.0f-1.0f) * horizontalScale,
                  fx = (static_cast<float>(x)/dimension * 2.0f-1.0f) * horizontalScale;
            float y = perlinNoise(fz/10, fx/10, 0)*5;
            vertices.push_back( Vertex{glm::vec3{fx, y, fz}, glm::vec2{fx, fz}} );
        }
    }

    std::vector<unsigned int> indices;
    int squareDim = dimension-1;
    for (int y=0; y<squareDim; y++)
    {
        for (int x=0; x<squareDim; x++)
        {
            int tl=((y+0)*dimension)+(x+0),
                tr=((y+0)*dimension)+(x+1),
                bl=((y+1)*dimension)+(x+0),
                br=((y+1)*dimension)+(x+1);
            
            indices.push_back(tl);
            indices.push_back(bl);
            indices.push_back(br);

            indices.push_back(br);
            indices.push_back(tr);
            indices.push_back(tl);
        }
    }

    return Mesh{vertices, indices};
}
