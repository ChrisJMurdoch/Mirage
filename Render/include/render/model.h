
#pragma once

#include "render\program.h"
#include "generate\mesh.h"

#include <glm\glm.hpp>

class Model
{
public:
	static const int STRIDE=9, POSITION_INDEX=0, NORMAL_INDEX=3, COLOUR_INDEX=6;
private:
	int nVertices, nIndices;
	unsigned int VBO, VAO, EBO;
	Program *prog;
public:
	Model(Mesh *mesh, Program *prog);
	void render(glm::mat4 &view, glm::mat4 &projection);
	~Model();
};
