
#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 lPos;
layout (location = 1) in vec2 lTexPos;
layout (location = 2) in vec3 lNorm;

out vec2 vTexPos;
out float light;

void main()
{
    gl_Position = projection * view * model * vec4(lPos, 1.0f);
    vTexPos = lTexPos;
    light = dot(vec3(0.5f, 0.9f, 0.2f), lNorm);
}
