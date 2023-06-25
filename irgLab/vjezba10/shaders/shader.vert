#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 posLightSpace;

uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
   vec4 wolrdPosLightSpace = model * vec4(aPos, 1.0);

   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

   TexCoords = aTexCoords;

   vec4 hpostLightSpace = lightProjection * lightView * wolrdPosLightSpace;
   posLightSpace = hpostLightSpace.xyz / hpostLightSpace.w;

   FragPos = wolrdPosLightSpace.xyz / wolrdPosLightSpace.w;

   gl_Position = projection * view * wolrdPosLightSpace;
}