
#include "arrayBuffer/vertexBuffer.hpp"

#include <iostream>

// PNC : VertexBuffer
PNC::PNC(int length) : VertexBuffer(length*STRIDE)
{
}
void PNC::bufferData() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    setAttribute(0, 3, GL_FLOAT, STRIDE, POS_PTR); // Position
    setAttribute(1, 3, GL_FLOAT, STRIDE, NOR_PTR); // Normal
    setAttribute(2, 3, GL_FLOAT, STRIDE, COL_PTR); // Colour
    glBufferData(GL_ARRAY_BUFFER, (long long)length*sizeof(float), data, GL_STATIC_DRAW);
}
Vec3<float> *PNC::position(int index) { return (Vec3<float> *)&data[(index*STRIDE)+POS_PTR]; }
Vec3<float> *PNC::normal(int index)   { return (Vec3<float> *)&data[(index*STRIDE)+NOR_PTR]; }
Vec3<float> *PNC::colour(int index)   { return (Vec3<float> *)&data[(index*STRIDE)+COL_PTR]; }
Vec3<float> const *PNC::position(int index) const { return (Vec3<float> *)&data[(index*STRIDE)+POS_PTR]; }
Vec3<float> const *PNC::normal(int index)   const { return (Vec3<float> *)&data[(index*STRIDE)+NOR_PTR]; }
Vec3<float> const *PNC::colour(int index)   const { return (Vec3<float> *)&data[(index*STRIDE)+COL_PTR]; }

// Tri : VertexBuffer
Tri::Tri(int length) : VertexBuffer(length*STRIDE)
{
}
void Tri::bufferData() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)length*sizeof(unsigned int), data, GL_STATIC_DRAW);
}
Vec3<unsigned int> *Tri::tri(int index) { return (Vec3<unsigned int> *)&data[(index*STRIDE)+TRI_PTR]; }
Vec3<unsigned int> const *Tri::tri(int index) const { return (Vec3<unsigned int> *)&data[(index*STRIDE)+TRI_PTR]; }
