
#version 460 core

uniform vec3 uniformColour;

out vec4 fragColour;

void main()
{
    fragColour = vec4(uniformColour, 1.0f);
}
