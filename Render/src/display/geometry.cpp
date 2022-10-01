
#include "display/geometry.hpp"

#include "display/gl.hpp"

#include <cstddef>

void Vertex::setAttributes()
{
    Vertex::setFloatAttr( 0, sizeof(position), offsetof(Vertex, position) );
    Vertex::setFloatAttr( 1, sizeof(texturePosition), offsetof(Vertex, texturePosition) );
    Vertex::setFloatAttr( 2, sizeof(normal), offsetof(Vertex, normal) );
    Vertex::setFloatAttr( 3, sizeof(tangent), offsetof(Vertex, tangent) );
    Vertex::setFloatAttr( 4, sizeof(bitangent), offsetof(Vertex, bitangent) );
}

void Vertex::setFloatAttr(unsigned int position, std::size_t size, std::size_t offset)
{
    glVertexAttribPointer(position, size/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offset));
    glEnableVertexAttribArray(position);
}
