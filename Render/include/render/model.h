
#pragma once

#include "render\program.h"
#include "generate\mesh.h"

#include <glm\glm.hpp>

class Model
{
public:
	static const int STRIDE=6, COORD_INDEX=0, COLOUR_INDEX=3;
private:
	int nVertices, nIndices;
	unsigned int VBO, VAO, EBO;
	Program *prog;
public:
	Model(Mesh *mesh, Program *prog);
	void render(glm::mat4 &view, glm::mat4 &projection);
	~Model();
};
