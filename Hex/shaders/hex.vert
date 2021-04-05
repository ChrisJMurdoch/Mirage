
#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 l_position;
layout(location = 1) in vec3 l_normal;
layout(location = 2) in vec3 l_colour;
layout(location = 3) in vec3 i_position;
layout(location = 4) in vec4 i_orientation;

out vec3 position;
out vec3 normal;
out vec3 colour;

void main()
{
    gl_Position = projection * view * model * vec4(l_position+i_position, 1.0);

    position = vec3(model * vec4(l_position, 1.0));
    normal = mat3(transpose(inverse(model))) * l_normal;
    colour = l_colour;
}
