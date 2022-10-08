
#version 460 core

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D bakedMap;

in vec3 vWorldPos;
in vec2 vTexPos;
in mat3 vTbn;

out vec4 fragColour;

void main()
{
    // Combined light
    vec3 light = texture(bakedMap, vTexPos).xyz;

    // Calculate colour
    fragColour = texture(albedoMap, vTexPos) * vec4(light, 1.0);
}
