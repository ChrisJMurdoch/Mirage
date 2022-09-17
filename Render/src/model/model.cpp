
#include "model/model.hpp"

#include "shader/program.hpp"
#include "display/gl.hpp"

Model::Model(Program const &program, std::vector<Vec3> const &vertices, std::vector<unsigned int> const &indices) : program(program)
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind
    gl::bindVertexArray(0);
}

Model::Model(Model &&other) : program(other.program), vao(other.vao), vbo(other.vbo), ebo(other.ebo)
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
        gl::bindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        gl::bindVertexArray(0);
    });
}
