#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
public:
    Material(){
        this->ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        this->diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
        this->shininess = 0.6f;
    }

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

    glm::vec3 getAmbient(){
        return ambient;
    }

    glm::vec3 getDiffuse(){
        return diffuse;
    }

    glm::vec3 getSpecular(){
        return specular;
    }

    float getShininess(){
        return shininess;
    }
};
