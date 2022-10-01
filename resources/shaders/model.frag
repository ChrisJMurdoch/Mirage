
#version 460 core
 
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;

in vec2 vTexPos;
in mat3 vTbn;

out vec4 fragColour;

void main()
{
    // Calculate normal and transform to world space
    vec3 tangentSpaceNormal = texture(normalMap, vTexPos).rgb * 2.0 - 1.0; // Map [0,1] -> [-1,1]
    vec3 normal = normalize( vTbn * tangentSpaceNormal );

    // Calculate light
    float light = dot(vec3(0.5f, 0.9f, 0.2f), normal);

    // Calculate colour
    fragColour = texture(albedoMap, vTexPos) * light;
}
