#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

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

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightIntensity;

void main()
{
   FragPos = vec3(model * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(model))) * aNormal;

   vec3 lightDir = normalize(light.position - FragPos);

   float diff = max(dot(Normal, lightDir), 0.0);

   vec3 reflectDir = reflect(-lightDir, Normal);

   float spec = pow(max(dot(viewPos, reflectDir), 0.0), material.shininess);

   vec3 ambient = light.ambient * material.ambient;

   vec3 diffuse = light.diffuse * diff * material.diffuse;

   vec3 specular = light.specular * spec * material.specular;

   LightIntensity = ambient + diffuse + specular;
   gl_Position = projection * view * model * vec4(aPos, 1.0);
}