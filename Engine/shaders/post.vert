
#version 460 core

layout (location = 0) in vec2 l_screenPosition;
layout (location = 1) in vec2 l_texPosition;

out vec2 screenPosition;
out vec2 texPosition;

void main()
{
    gl_Position = vec4(l_screenPosition.xy, 0, 1);
    texPosition = l_texPosition;
    screenPosition = l_screenPosition;
}
