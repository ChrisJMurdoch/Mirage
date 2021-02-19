
#version 460 core

const vec3 lightPosition = vec3(5,5,5);

in vec3 position;
in vec3 normal;
in vec3 colour;

out vec4 fragColour;

void main()
{
    // Ambient
    float ambient = 0.1;
  	
    // Diffuse 
    vec3 lightDir = normalize(lightPosition - position);
    float diffuse = max(dot(normal, lightDir), 0.0);
    
    vec3 result = (ambient + diffuse) * colour;
    fragColour = vec4(result, 1.0);
}
