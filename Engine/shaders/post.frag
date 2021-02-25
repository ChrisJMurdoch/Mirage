
#version 460 core

uniform sampler2D colour;
uniform sampler2D depth;

uniform vec3 planet_centre;
uniform mat4 projection_inverse;
uniform mat4 view_inverse;

in vec2 texPosition;
in vec2 screenPosition;

out vec4 fragColour;

void main()
{
    vec4 clipspace = vec4( screenPosition.xy, texture(depth, texPosition).x * 2 - 1, 1 );

    // Clipspace -> viewspace
    vec4 viewspace = projection_inverse * clipspace;
    viewspace /= viewspace.w;

    // Viewspace -> worldspace
    vec4 worldspace = view_inverse * viewspace;

    // Distance from planet centre
    float height = length(worldspace.xyz-planet_centre);

    fragColour = height<1 ? (vec4(0,0.4,1,1)+texture(colour, texPosition))/2 : texture(colour, texPosition);
}
