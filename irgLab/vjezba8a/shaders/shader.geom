#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in VS_OUT {
    vec3 point;
} gs_in[];

out vec3 FragPos;

void calcLight()
{

    vec3 face = normalize(cross(gs_in[1].point - gs_in[0].point, gs_in[2].point - gs_in[0].point));

    if (dot(face, gs_in[0].point - viewPos) < 0){

        vec3 v1 = gs_in[1].point - gs_in[0].point;
        vec3 v2 = gs_in[2].point - gs_in[0].point;

        vec3 normal = normalize(cross(v1, v2));

        vec3 center = (gs_in[0].point + gs_in[1].point + gs_in[2].point) / 3.0;

        vec3 lightDir = normalize(light.position - center);    

        vec3 viewDir = normalize(viewPos - center);

        vec3 ambient = light.ambient * material.ambient;

        float diff = max(dot(normal, lightDir), 0.0);

        vec3 diffuse = light.diffuse * (diff * material.diffuse);

        vec3 reflectDir = reflect(-lightDir, normal);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 specular = light.specular * (spec * material.specular);

        FragPos = specular + diffuse + ambient;
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();

        gl_Position = gl_in[1].gl_Position;
        EmitVertex();

        gl_Position = gl_in[2].gl_Position;
        EmitVertex();

        EndPrimitive();
    }
}

void main()
{
    calcLight();
}