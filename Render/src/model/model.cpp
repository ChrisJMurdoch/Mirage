
#include "model/model.hpp"

#include "display/gl.hpp"
#include "shader/program.hpp"
#include "texture/texture.hpp"

#include <iostream>

void Vertex::setAttributes()
{
    // Position (vec3)
    glVertexAttribPointer( 0, sizeof(position)/sizeof(float),        GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position))        );
    glEnableVertexAttribArray(0);

    // Texture Position (vec2)
    glVertexAttribPointer( 1, sizeof(texturePosition)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texturePosition)) );
    glEnableVertexAttribArray(1);
}

Model::Model(Program const &program, Texture const &texture, std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices) : program(program), texture(texture), nIndices(indices.size())
{
    // Generate buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Bind VAO
    gl::bindVertexArray(vao);
    
    // Bind buffers to VAO
    gl::bindArrayBuffer(vbo);
    gl::bindElementArrayBuffer(ebo);

    // Copy over data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(), indices.data(), GL_STATIC_DRAW);

    // Set attributes
    Vertex::setAttributes();

    // Unbind
    gl::bindVertexArray(0);
}

Model::Model(Model &&other) : program(other.program), texture(texture), nIndices(other.nIndices), vao(other.vao), vbo(other.vbo), ebo(other.ebo)
{
    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
}

Model::~Model()
{
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Model::draw() const
{
    program.use([&]()
    {
        texture.use(GL_TEXTURE0, [&]()
        {
            gl::bindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
            gl::bindVertexArray(0);
        });
    });
}
