
#include "model/model.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Model::Model(VertexArray *vertexArray, IndexArray *indexArray, Program *prog)
{
    nVertices = vertexArray->getNVertices();
    nIndices = indexArray->getNIndices();
    this->prog = prog;

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind data to VAO
    glBindVertexArray(VAO);
    {
        // Buffer vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (long long)nVertices*STRIDE*sizeof(float), vertexArray->getArrayPointer(), GL_STATIC_DRAW);

        // Buffer index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices*sizeof(unsigned int), indexArray->getArrayPointer(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE*sizeof(float), (void *)(POSITION_INDEX*sizeof(float)));
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE*sizeof(float), (void *)(NORMAL_INDEX*sizeof(float)));
        glEnableVertexAttribArray(1);

        // Colour attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, STRIDE*sizeof(float), (void *)(COLOUR_INDEX*sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    glBindVertexArray(0);

    std::cout << "+Model" << std::endl;
}

void Model::render(glm::mat4 &view, glm::mat4 &projection)
{
    // Matrices
    static const glm::mat4 IDENTITY = glm::mat4(1.0f);
    static float rotate = 0.0f;
    rotate += 1;
    glm::mat4 model = glm::rotate(IDENTITY, glm::radians(rotate), glm::vec3(0.0f, 0.5f, 0.5f));

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
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    std::cout << "~Model" << std::endl;
}
