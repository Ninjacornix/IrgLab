#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightProjection;
uniform mat4 lightView;

out vec4 FragPos;

void main()
{
    gl_Position = lightProjection * lightView * model * vec4(aPos, 1.0);
    FragPos = gl_Position;
}