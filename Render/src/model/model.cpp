
#include "model/model.hpp"

#include <glad/glad.h>

Model::Model(VertexBuffer<float> const *vertexBuffer, VertexBuffer<unsigned int> const *indexBuffer, VertexBuffer<float> const *instanceBuffer, Program *prog) : vertexBuffer(vertexBuffer), indexBuffer(indexBuffer), instanceBuffer(instanceBuffer), prog(prog)
{
    // Bind data to VAO and buffer
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    indexBuffer->bufferData();
    vertexBuffer->bufferData();
    instanceBuffer->bufferData();
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
    glDrawElementsInstanced(GL_TRIANGLES, indexBuffer->getLength(), GL_UNSIGNED_INT, 0, instanceBuffer->getElementLength());
}

Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
}
