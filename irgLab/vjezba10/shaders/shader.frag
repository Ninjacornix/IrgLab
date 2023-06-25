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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 posLightSpace;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform sampler2D shadowMap;
uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main()
{
   vec3 posLightUV = posLightSpace * 0.5 + 0.5;
   float closestDepth = texture(shadowMap, posLightUV.xy).r;
   float currentDepth = posLightSpace.z * 0.5 + 0.5;

   vec3 vertexLight = normalize(lightPos - FragPos);
   vec3 cameraLight = normalize(viewPos - FragPos);
   vec3 halfVector = normalize((vertexLight + cameraLight)/ 2.0);

   vec3 totalAmbient = light.ambient * material.ambient;

   float diff = max(dot(Normal, vertexLight), 0.0);

   vec3 totalDiffuse = light.diffuse * (diff * material.diffuse);

   float spec = pow(max(dot(Normal, halfVector), 0.0), material.shininess);

   vec3 totalSpecular = light.specular * (spec * material.specular);

   vec3 totalLight = totalAmbient + totalDiffuse + totalSpecular;

   vec3 texColor = texture(texture_diffuse1, TexCoords).rgb;

   if (currentDepth > closestDepth)
   {
      totalLight *= 0.5;
   }

   FragColor = vec4(texColor * totalLight, 1.0);
}