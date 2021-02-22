
#version 460 core

const vec3 lightPosition = vec3(3,5,3);

in vec3 position;
in vec3 normal;
in vec3 colour;

out vec4 fragColour;

void main()
{
    // Fix values
    vec3 colour = clamp(colour, 1, 1);
    vec3 normal = normalize(normal);

    // Ambient
    float ambient = 0.1;
  	
    // Diffuse 
    vec3 lightDir = normalize(lightPosition - position);
    float diffuse = max(dot(normal, lightDir), 0);
    
    vec3 result = (ambient + diffuse) * colour;
    fragColour = vec4(0, 0.3, 0.5, 0.5);
}
