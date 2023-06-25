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

    vector<glm::mat4> getPerspectiveMatrix(int width, int height){
        glm::mat4 view = glm::lookAt(eye, center, up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 modelMatrix = getModelMatrix();
        vector<glm::mat4> matrices;
        matrices.push_back(view);
        matrices.push_back(projection);
        matrices.push_back(modelMatrix);
        return matrices;
    }

};