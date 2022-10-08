
#version 460 core

const vec3 cLightPosition = vec3(2,5,1);

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
    // Normal mapping
    vec3 tangentSpaceNormal = texture(normalMap, vTexPos).rgb * 2.0 - 1.0; // Map [0,1] -> [-1,1]
    vec3 normal = normalize( vTbn * tangentSpaceNormal );

    // Ambient light
    vec3 ambient = vec3(0.1);

    // Diffuse light
    vec3 lightDir = normalize(cLightPosition - vWorldPos);
    float diffuse = max(dot(normal, lightDir), 0);

    // Combined light
    vec3 light = clamp(ambient+diffuse, 0, 1);

    // Calculate colour
    fragColour = texture(albedoMap, vTexPos) * vec4(light, 1.0);
}
