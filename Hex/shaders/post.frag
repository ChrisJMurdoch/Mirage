
#version 460 core

uniform sampler2D colour;
uniform sampler2D depth;

uniform mat4 projection_inverse;
uniform mat4 view_inverse;

in vec2 texPosition;
in vec2 screenPosition;

out vec4 fragColour;

void main()
{
    fragColour = texture(colour, texPosition);
}
