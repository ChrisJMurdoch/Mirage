
#version 460 core

const int SAMPLES = 16; // Change in src

uniform sampler2DMS colour;
uniform sampler2DMS depth;

uniform mat4 projection_inverse;
uniform mat4 view_inverse;

in vec2 texPosition;
in vec2 screenPosition;

out vec4 fragColour;

void main()
{
    // Get texture coords
    ivec2 texSize = textureSize(colour);
    ivec2 iTexPos = ivec2(texSize.x*texPosition.x, texSize.y*texPosition.y);

    // Multisample
    vec4 acc = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i=0; i<SAMPLES; i++)
        acc += texelFetch(colour, iTexPos, i);
    acc /= float(SAMPLES);

    fragColour = acc;
}
