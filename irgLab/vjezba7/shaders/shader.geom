#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 point;
} gs_in[];

uniform vec3 eye;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void FaceCulling()
{
    gl_Position = projection * view * model * vec4(gs_in[0].point, 1.0f);
    EmitVertex();

    gl_Position = projection * view * model * vec4(gs_in[1].point, 1.0f);
    EmitVertex();

    gl_Position = projection * view * model * vec4(gs_in[2].point, 1.0f);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    FaceCulling();
}