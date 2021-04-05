
#pragma once

#include "arrayBuffer/vertexBuffer.hpp"
#include "render/program.hpp"

#include <glm/glm.hpp>

class Model
{
private:
	unsigned int VAO;
	VertexBuffer<float> const *vertexBuffer;
	VertexBuffer<unsigned int> const *indexBuffer;
	VertexBuffer<float> const *instanceBuffer;
	Program *prog;
public:
	Model(VertexBuffer<float> const *vertexBuffer, VertexBuffer<unsigned int> const *indexBuffer, VertexBuffer<float> const *instanceBuffer, Program *prog);
	void render(glm::mat4 const &model, glm::mat4 const &view, glm::mat4 const &projection) const;
	~Model();
};
