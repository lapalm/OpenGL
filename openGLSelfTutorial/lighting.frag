#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;  

void main()
{
	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

	// Use the max function that returns the highest of both its parameters to make sure the diffuse coponent (and color) never becomes negative.
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular Lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // shininess = power of (32 for example). Value of the shininess value of the highlight. Higher the value the more properly we reflect the light instead of scattering it all around.
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular);
	color = vec4(result, 1.0f);
}