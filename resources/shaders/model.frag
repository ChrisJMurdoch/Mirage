
#version 460 core
 
uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D roughness;

in vec2 vTexPos;
in float light;

out vec4 fragColour;

void main()
{
    fragColour = texture(albedo, vTexPos) * light;
}
