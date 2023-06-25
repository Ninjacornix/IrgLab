#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT {
    vec3 point;
} gs_in[];

uniform vec3 eye;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void FaceCulling()
{
    vec3 face = normalize(cross(gs_in[1].point - gs_in[0].point, gs_in[2].point - gs_in[0].point));

    
    if (dot(face, gs_in[0].point - eye) > 0)
    {
        gl_Position = projection * view * model * vec4(gs_in[0].point, 1.0);
        EmitVertex();
        gl_Position = projection * view * model * vec4(gs_in[1].point, 1.0);
        EmitVertex();
        gl_Position = projection * view * model * vec4(gs_in[2].point, 1.0);
        EmitVertex();
        gl_Position = projection * view * model * vec4(gs_in[0].point, 1.0);
        EmitVertex();
        EndPrimitive();
    }

}

void main()
{
    FaceCulling();
}