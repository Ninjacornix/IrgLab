#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 point;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void faceCullingVertex()
{
    vec4 temp = projection * view * model * vec4(aPos, 1.0);
    if(temp.w < 0.000001)
        temp.w = 1.0;
    gl_Position = temp / temp.w;
    vs_out.point = aPos;
}

void main()
{
    faceCullingVertex();
}