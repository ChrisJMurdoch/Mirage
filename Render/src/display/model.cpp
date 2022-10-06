
#include "display/model.hpp"

#include "display/gl.hpp"
#include "display/program.hpp"
#include "display/texture.hpp"
#include "display/geometry.hpp"

#include <iostream>

Model::Model(Program const &program, std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices, Material const &material)
    : program(program), material(material), nIndices(indices.size())
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

Model::Model(Model &&other)
    : program(other.program), material(material), nIndices(other.nIndices), vao(other.vao), vbo(other.vbo), ebo(other.ebo)
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
        if (material.albedo) material.albedo->bind(Texture::Channel::Albedo);
        if (material.normal) material.normal->bind(Texture::Channel::Normal);
        if (material.roughness) material.roughness->bind(Texture::Channel::Roughness);
        {
            gl::bindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
            gl::bindVertexArray(0);
        }
        if (material.roughness) material.roughness->unbind(Texture::Channel::Roughness);
        if (material.normal) material.normal->unbind(Texture::Channel::Normal);
        if (material.albedo) material.albedo->unbind(Texture::Channel::Albedo);
    });
}
