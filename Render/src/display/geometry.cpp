
#include "display/geometry.hpp"

#include "display/gl.hpp"

#include <cstddef>

void Vertex::setAttributes()
{
    // Position (vec3)
    glVertexAttribPointer( 0, sizeof(position)/sizeof(float),        GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position))        );
    glEnableVertexAttribArray(0);

    // Texture Position (vec2)
    glVertexAttribPointer( 1, sizeof(texturePosition)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texturePosition)) );
    glEnableVertexAttribArray(1);

    // Normal Position (vec3)
    glVertexAttribPointer( 2, sizeof(normal)/sizeof(float),          GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal))          );
    glEnableVertexAttribArray(2);
}
