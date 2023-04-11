
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
    // Read albedo
    vec4 albedo = texture(albedoMap, vTexPos);

    // Read lightmap
    vec3 light = texture(bakedMap, vTexPos).xyz;

    // Calculate colour
    fragColour = albedo * vec4(light, 1.0);
}
