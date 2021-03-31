
#pragma once

#include "render/program.hpp"
#include "arrayBuffer/vertexBuffer.hpp"

#include <glm/glm.hpp>

class Model
{
public:
	static const int STRIDE=9, POSITION_INDEX=0, NORMAL_INDEX=3, COLOUR_INDEX=6;
private:
	int nIndices;
	unsigned int VAO;
	Program *prog;
public:
	Model(VertexBuffer<float> const *vertexBuffer, VertexBuffer<int> const *indexBuffer, Program *prog);
	void render(glm::mat4 const &model, glm::mat4 const &view, glm::mat4 const &projection) const;
	~Model();
};
