
#pragma once

#include "model/vertexArray.hpp"
#include "model/indexArray.hpp"
#include "render/program.hpp"

#include <glm/glm.hpp>

class Model
{
public:
	static const int STRIDE=9, POSITION_INDEX=0, NORMAL_INDEX=3, COLOUR_INDEX=6;
private:
	int nVertices, nIndices;
	unsigned int VBO, VAO, EBO;
	Program *prog;
public:
	Model(VertexArray *vertexArray, IndexArray *indexArray, Program *prog);
	void render(glm::mat4 const &model, glm::mat4 const &view, glm::mat4 const &projection) const;
	~Model();
};
