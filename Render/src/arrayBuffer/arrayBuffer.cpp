
#include <glad/glad.h>

#include "arrayBuffer/arrayBuffer.hpp"
#include <iostream>

// ArrayBuffer

ArrayBuffer::ArrayBuffer(int size) : data(new float[size]), size(size)
{
    // glGenBuffers(1, &id);
}
void ArrayBuffer::setAttribute(int index, int size, int stride, int ptr)
{
    glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void *)(ptr*sizeof(float)) );
    glEnableVertexAttribArray(index);
}

// PNC : ArrayBuffer

PNC::PNC(int size) : ArrayBuffer(size*STRIDE)
{
}
void PNC::bufferData(unsigned int vao)
{
    // Bind data to VAO
    //glBindVertexArray(vao);
    {
        // Buffer data
        for (int i=0; i<size/STRIDE; i++)
            std::cout << position(i)->s() << " " << normal(i)->s() << " " << colour(i)->s() << std::endl;
        //glBindBuffer(GL_ARRAY_BUFFER, id);
        //glBufferData(GL_ARRAY_BUFFER, (long long)size*sizeof(float), data, GL_STATIC_DRAW);

        // Set attributes
        //setAttribute(0, 3, STRIDE, POS_PTR); // Position
        //setAttribute(1, 3, STRIDE, NOR_PTR); // Normal
        //setAttribute(2, 3, STRIDE, COL_PTR); // Colour
    }
    //glBindVertexArray(0);
}
Vec3F *PNC::position(int index)
{
    return (Vec3F *)&data[(index*STRIDE)+POS_PTR];
}
Vec3F *PNC::normal(int index)
{
    return (Vec3F *)&data[(index*STRIDE)+NOR_PTR];
}
Vec3F *PNC::colour(int index)
{
    return (Vec3F *)&data[(index*STRIDE)+COL_PTR];
}
