
#include "model/model.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Model::Model(VertexBuffer<float> const *vertexBuffer, VertexBuffer<unsigned int> const *indexBuffer, Program *prog)
{
    nIndices = indexBuffer->getLength();
    this->prog = prog;

    // Bind data to VAO and buffer
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    indexBuffer->bufferData();
    vertexBuffer->bufferData();
}

void Model::render(glm::mat4 const &model, glm::mat4 const &view, glm::mat4 const &projection) const
{
    // Set uniforms
    prog->setUniformMatrix4fv("model", model);
    prog->setUniformMatrix4fv("view", view);
    prog->setUniformMatrix4fv("projection", projection);

    // Draw
    prog->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
}
