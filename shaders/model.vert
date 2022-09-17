
#version 460 core

layout (location = 0) in vec3 lPos;
layout (location = 1) in vec2 lTexPos;

out vec2 vTexPos;

void main()
{
    gl_Position = vec4(lPos, 1.0);
    vTexPos = lTexPos;
}
