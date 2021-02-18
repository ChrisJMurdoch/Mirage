
#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

out vec4 vertColour;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	vertColour = vec4(colour, 1);
}
