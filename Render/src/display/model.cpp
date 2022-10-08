
#include "display/model.hpp"

#define UTILITY_GL_FUNCTIONS
#include "utility/geometry.hpp"

#include "display/gl.hpp"
#include "display/program.hpp"
#include "display/texture.hpp"

#include <iostream>
#include <cstddef>

void setFloatAttr(unsigned int position, std::size_t size, std::size_t offset)
{
    glVertexAttribPointer(position, size/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offset));
    glEnableVertexAttribArray(position);
}

Model::Model(Program const &program, Mesh const &mesh, Material const &material)
    : program(program), material(material), nIndices(mesh.indices.size())
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices[0])*mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices[0])*mesh.indices.size(), mesh.indices.data(), GL_STATIC_DRAW);

    // Set attributes
    setFloatAttr( 0, sizeof(Vertex::pos), offsetof(Vertex, pos) );
    setFloatAttr( 1, sizeof(Vertex::uv), offsetof(Vertex, uv) );
    setFloatAttr( 2, sizeof(Vertex::norm), offsetof(Vertex, norm) );
    setFloatAttr( 3, sizeof(Vertex::tan), offsetof(Vertex, tan) );
    setFloatAttr( 4, sizeof(Vertex::btan), offsetof(Vertex, btan) );

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
        if (material.baked) material.baked->bind(Texture::Channel::Baked);
        {
            gl::bindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
            gl::bindVertexArray(0);
        }
        if (material.baked) material.baked->unbind(Texture::Channel::Baked);
        if (material.roughness) material.roughness->unbind(Texture::Channel::Roughness);
        if (material.normal) material.normal->unbind(Texture::Channel::Normal);
        if (material.albedo) material.albedo->unbind(Texture::Channel::Albedo);
    });
}
