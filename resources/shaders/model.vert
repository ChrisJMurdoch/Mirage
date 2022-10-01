
#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 lPos;
layout (location = 1) in vec2 lTexPos;
layout (location = 2) in vec3 lNorm;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec3 lBitangent;

out vec3 vWorldPos;
out vec2 vTexPos;
out mat3 vTbn;

void main()
{
    gl_Position = projection * view * model * vec4(lPos, 1.0f);
    vWorldPos = vec3(model * vec4(lPos, 1.0));
    vTexPos = lTexPos;
    vTbn = mat3(
        normalize(vec3(model * vec4(lTangent, 0.0))),
        normalize(vec3(model * vec4(lBitangent, 0.0))),
        normalize(vec3(model * vec4(lNorm, 0.0)))
    );
}
