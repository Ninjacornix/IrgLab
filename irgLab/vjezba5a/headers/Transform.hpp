#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once

class Transform
{
    private:
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    public:

        Transform(){
            modelMatrix = glm::mat4(1.0f);
        }

        void setPosition(glm::vec3 position){
            this->position = position;
        }

        void setOrientation(glm::vec3 orientation){
            this->orientation = orientation;
        }

        glm::mat4 getViewMatrix(){
            return viewMatrix;
        }

        glm::mat4 getModelMatrix(){
            return modelMatrix;
        }

        glm::vec3 translate(glm::vec3 point, glm::vec3 translation){
            translation = -1.0f * translation;
            return point + translation;
        }

        glm::vec3 scaler(glm::vec3 point, float delta){
            return point * delta;
        }

        glm::vec3 apply(glm::vec3 point, glm::vec3 translation, float M){
            glm::vec3 p = translate(point, translation);
            return scaler(p, M);
        }
};
