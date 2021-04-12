
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
    // World space
    position = vec3(model * vec4(l_position+i_position, 1.0));

    // Clip space
    gl_Position = projection * view * vec4(position, 1.0);

    // Normal
    normal = mat3(transpose(inverse(model))) * l_normal;
    
    // Colour
    vec3 dither = floatHash( int(i_position.x*1000+i_position.z*10) ) * vec3(0.1f);
    colour = l_position.y>0.0f ? vec3(1.0f) : vec3(0.0f);
    switch (int(i_position.y*10))
    {
    case 0:
        colour = vec3(0.0f, 0.5f, 0.8f) + dither;
        break;
    case 1:
        colour = vec3(0.8f, 0.8f, 0.6f) + dither;
        break;
    case 2:
        colour = vec3(0.3f, 0.6f, 0.0f) + dither;
        break;
    case 3:
        colour = vec3(0.3f, 0.3f, 0.3f) + dither;
        break;
    default:
        colour = vec3(1.0f, 0.0f, 0.0f) + dither;
    }
}

/* Quick hashing function */
float floatHash(int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return (x % 10000) / 9999.0f;
}
