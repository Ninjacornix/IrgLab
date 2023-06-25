#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 point;
} vs_out;

void faceCullingVertex()
{
    vs_out.point = aPos;
    gl_Position = vec4(aPos, 1.0);
}

void main()
{
    faceCullingVertex();
}