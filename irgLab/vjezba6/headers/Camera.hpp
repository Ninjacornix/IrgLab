#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "Transform.hpp"

using namespace std;

class Camera : public Transform
{
private:
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
public:
    Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up) : eye(eye), center(center), up(up) {}
    ~Camera(){}

    vector<glm::mat4> getPerspectiveMatrix(){
        glm::mat4 view = lookAtMatrix(eye, center, up);
        glm::mat4 projection = frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 100.0f);
        glm::mat4 modelMatrix = getModelMatrix();
        vector<glm::mat4> matrices;
        matrices.push_back(view);
        matrices.push_back(projection);
        matrices.push_back(modelMatrix);
        return matrices;
    }

    glm::vec3 getEye(){
        return eye;
    }

    glm::vec3 getCenter(){
        return center;
    }

    glm::vec3 getUp(){
        return up;
    }

    void moveForward(float speed){
        glm::vec3 direction = glm::normalize(center - eye);
        eye += direction * speed;
        center += direction * speed;
    }

    void moveRight(float speed){
        glm::vec3 direction = glm::normalize(center - eye);
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        eye += right * speed;
        center += right * speed;
    }

    void moveUp(float speed){
        glm::vec3 direction = glm::normalize(center - eye);
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        glm::vec3 up = glm::normalize(glm::cross(right, direction));
        eye += up * speed;
        center += up * speed;
    }

    void rotate(float angle, glm::vec3 axis){
        glm::vec3 direction = glm::normalize(center - eye);
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        glm::vec3 up = glm::normalize(glm::cross(right, direction));
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
        glm::vec4 newDirection = rotationMatrix * glm::vec4(direction, 1.0f);
        glm::vec4 newUp = rotationMatrix * glm::vec4(up, 1.0f);
        center = eye + glm::vec3(newDirection);
        up = glm::vec3(newUp);
    }

};