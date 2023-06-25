#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 color;

uniform mat4 tMatrica;

void main()
{
	color = aCol;
    gl_Position = tMatrica*vec4(aPos, 1.0);
}