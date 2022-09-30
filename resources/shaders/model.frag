
#version 460 core
 
uniform sampler2D tex;

in vec2 vTexPos;
in float light;

out vec4 fragColour;

void main()
{
    fragColour = texture(tex, vTexPos) * light;
}
