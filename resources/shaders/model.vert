
#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 lPos;
layout (location = 1) in vec2 lTexPos;

out vec2 vTexPos;

void main()
{
    gl_Position = projection * view * model * vec4(lPos, 1.0f);
    vTexPos = lTexPos;
}
