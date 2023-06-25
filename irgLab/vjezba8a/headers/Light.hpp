#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <string>

#include "Transform.hpp"

#pragma once

#define STB_IMAGE_IMPLEMENTATION

class Light : public Transform
{
private:
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    glm::vec3 lightAmbient;
    glm::vec3 lightDiffuse;
    glm::vec3 lightSpecular;
    float constant;
public:
    Light(){
        lightPos = glm::vec3(1.0f, 1.0f, 2.0f);
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        lightAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
        lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
        constant = 1.0f;
    }

    Light(glm::vec3 lightPos, glm::vec3 lightColor, glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular, float constant) : lightPos(lightPos), lightColor(lightColor), lightAmbient(lightAmbient), lightDiffuse(lightDiffuse), lightSpecular(lightSpecular), constant(constant) {}

    glm::vec3 getLightPos(){
        return lightPos;
    }

    glm::vec3 getLightColor(){
        return lightColor;
    }

    glm::vec3 getLightAmbient(){
        return lightAmbient;
    }

    glm::vec3 getLightDiffuse(){
        return lightDiffuse;
    }

    glm::vec3 getLightSpecular(){
        return lightSpecular;
    }

    float getConstant(){
        return constant;
    }
};
