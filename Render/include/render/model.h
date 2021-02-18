
#pragma once

#include "render\program.h"
#include "generate\mesh.h"

#include <glm\glm.hpp>

class Model
{
private:
	int nVertices, nIndices;
	unsigned int VBO, VAO, EBO;
	Program *prog;
public:
	Model(Mesh *mesh, Program *prog);
	void render(glm::mat4 &view, glm::mat4 &projection);
	~Model();
};
