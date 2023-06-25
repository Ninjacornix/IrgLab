#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT {
    vec3 point;
} gs_in[];

uniform vec3 eye;

void FaceCulling()
{
    // determine if vertices are clockwise winded or counter-clockwise winded

    vec3 a = gs_in[0].point - eye;
    vec3 b = gs_in[1].point - eye;
    vec3 c = gs_in[2].point - eye;

    vec3 normal = normalize(cross(b - a, c - a));

    if (dot(normal, a) < 0)
    {
        for (int i = 0; i < gl_in.length(); i++)
        {
            gl_Position = gl_in[i].gl_Position;
            EmitVertex();
        }
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
    
}

void main()
{
    FaceCulling();
}