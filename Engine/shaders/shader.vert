
#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colour;

out vec4 vertColour;

void main()
{
	// Position
	gl_Position = projection * view * model * vec4(position, 1.0);

	// Colour
	vec4 wn = model * vec4(normal, 1.0); // Transform normal into world space
	float lum = dot( vec3(wn.x, wn.y, wn.z), vec3(0.0f,1.0f,0.0f) ); // Get upward-facing value
	vertColour = vec4(colour.r*lum, colour.g*lum, colour.b*lum, 1);
}
