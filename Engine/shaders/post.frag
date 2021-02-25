
#version 460 core

uniform sampler2D colour;
uniform sampler2D depth;

uniform vec3 planet_centre;
uniform mat4 projection_inverse;
uniform mat4 view_inverse;

in vec2 texPosition;
in vec2 screenPosition;

out vec4 fragColour;

bool hit_sphere(vec3 center, float radius, vec3 origin, vec3 direction);

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

    const float WATER_HEIGHT = 1.0;

    // Sphere
    vec3 obs = vec3(0,0,2);
    bool hit = hit_sphere( planet_centre, WATER_HEIGHT, obs, normalize(worldspace.xyz-obs) );

    // Add water
    fragColour = ( hit && (height<WATER_HEIGHT || height>WATER_HEIGHT+1) ) ? (vec4(0,0.4,1,1)+texture(colour, texPosition))/2 : texture(colour, texPosition);
}

bool hit_sphere(vec3 center, float radius, vec3 origin, vec3 direction)
{
    vec3 oc = origin - center;
    float a = dot(direction, direction);
    float b = 2.0 * dot(oc, direction);
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant>0);
}
