
#version 460 core

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertColor;

out vec3 colour;

void main()
{
    gl_Position = vec4(vertPosition, 1.0);
    colour = vertColor;
}
