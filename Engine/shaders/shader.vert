
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

out vec4 vertColour;

void main()
{
	gl_Position = vec4(position, 1);
	vertColour = vec4(colour, 1);
}
