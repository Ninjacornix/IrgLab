#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in mat4 tMatrica;

out vec3 color;

void main()
{
	color = aCol;
    gl_Position = tMatrica*vec4(aPos, 1.0);
}