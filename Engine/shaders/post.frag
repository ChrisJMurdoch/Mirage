
#version 460 core

uniform sampler2D colour;
uniform sampler2D depth;

uniform vec3 camera_position;
uniform vec3 planet_centre;
uniform mat4 projection_inverse;
uniform mat4 view_inverse;

in vec2 texPosition;
in vec2 screenPosition;

out vec4 fragColour;

vec3 raysphere(vec3 centre, float radius, vec3 origin, vec3 direction);

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

    // Calculate water depth
    vec3 direction = worldspace.xyz-camera_position;
    vec3 intersections = raysphere(planet_centre, WATER_HEIGHT, camera_position, direction);
    float water;
    if (intersections.x==0)
    {
        water = 0;
    }
    else
    {
        vec3 close = camera_position + (direction*intersections.y);
        vec3 far = camera_position + (direction*intersections.z);
        float closeDistance = length(close - camera_position);
        float farDistance = length(far - camera_position);
        float landDistance = length(worldspace.xyz - camera_position);
        water = max(0, min(farDistance, landDistance) - closeDistance);
    }
    water = min(1, water*20);

    // Add water to colour
    const vec4 OCEAN = vec4(0.0, 0.15, 0.3, 1.0);
    const vec4 SHORE = vec4(0.3, 0.4, 0.4, 1.0);
    if (water>0)
        fragColour = water*OCEAN + (1-water)*SHORE;
    else
        fragColour = texture(colour, texPosition);
}

/*
Returns vec3:
    X: Hit ? 1 : 0
    Y: t1
    Z: t2
*/
vec3 raysphere(vec3 centre, float radius, vec3 origin, vec3 direction)
{
    // Calculate discriminant
    vec3 offset = origin - centre;
    const float a = dot(direction, direction);
    float b = 2 * dot(offset, direction);
    float c = dot(offset, offset)  - (radius*radius);
    float discriminant = (b*b) - (4*a*c);

    // No hit or tangent hit
    if (discriminant <= 0)
        return vec3(0,0,0);

    // Calculate t1 & t2
    float s = sqrt(discriminant);
    float t1 = (-s-b) / (2*a);
    float t2 = (s-b) / (2*a);

    // Sphere is behind camera
    if (t2 <= 0)
        vec3(0,0,0);
    
    // Camera is inside sphere
    if (t1 <= 0)
        vec3(1, 0, t2);
    
    // Sphere is in front of camera
    return vec3(1, t1, t2);
}
