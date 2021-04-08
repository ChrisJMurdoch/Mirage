
#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 l_position;
layout(location = 1) in vec3 l_normal;
layout(location = 2) in vec3 l_colour;
layout(location = 3) in vec3 i_position;
layout(location = 4) in vec4 i_orientation;

out vec3 position;
out vec3 normal;
out vec3 colour;

float floatHash(int x);

void main()
{
    gl_Position = projection * view * model * vec4(l_position+i_position, 1.0);

    position = vec3(model * vec4(l_position+i_position, 1.0));
    normal = mat3(transpose(inverse(model))) * l_normal;
    float dither = floatHash(int(i_position.x+(1000*i_position.z)));
    colour = ( i_position.y <= 1.0f ? vec3(0.4f,0.4f,0.4f) : i_position.y <= 1.5f ? vec3(0.3f,0.6f,0.1f) : vec3(0.8f,0.8f,0.6f) ) + dither * vec3(0.1f, 0.1f, 0.1f);
}

/* Quick hashing function */
float floatHash(int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return (x % 10000) / 9999.0f;
}
