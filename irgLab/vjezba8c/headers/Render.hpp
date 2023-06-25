#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "Object.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

#pragma once

class Render
{
private:
    vector<Object> objects;
    Shader s = Shader();
public:
    Render(vector<Object> objects){
        this->objects = objects;
        string vertexSource = "/Users/ninjacornix/irglab/irgLab/vjezba8c/shaders/shader.vert";
        string fragmentSource = "/Users/ninjacornix/irglab/irgLab/vjezba8c/shaders/shader.frag";
        string geometrySource = "/Users/ninjacornix/irglab/irgLab/vjezba8c/shaders/shader.geom";
        s = Shader(vertexSource.c_str(), fragmentSource.c_str());
    }
    ~Render(){
        objects.clear();
    }
    Shader getShader(){
        return s;
    }

    void draw(){
        for (auto o : objects) {
            glUniformMatrix4fv(glGetUniformLocation(s.ID, "model"), 1, GL_FALSE, &o.meshes[0].transform.getModelMatrix()[0][0]);
            s.use();
            o.draw();
        }
    }

};