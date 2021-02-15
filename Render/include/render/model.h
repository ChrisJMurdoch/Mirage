
#pragma once

#include "render\program.h"

class Model
{
private:
	int nVertices, nIndices;
	unsigned int VBO, VAO, EBO;
	Program *prog;
public:
	Model(float *vertices, int nVertices, unsigned int *indices, int nIndices, Program *prog);
	void render();
	~Model();
};
