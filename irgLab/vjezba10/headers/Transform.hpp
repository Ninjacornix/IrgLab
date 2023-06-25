#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once

class Transform
{
    private:
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::mat4 modelMatrix = glm::mat4(1.0f);

    public:

        Transform(){
        }

        glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp){
            glm::vec3 f = glm::normalize(center - eye);
            glm::vec3 s = glm::normalize(glm::cross(f, viewUp));
            glm::vec3 u = glm::cross(s, f);
            glm::mat4 M = glm::mat4(1.0f);
            M[0][0] = s.x;
            M[1][0] = s.y;
            M[2][0] = s.z;
            M[0][1] = u.x;
            M[1][1] = u.y;
            M[2][1] = u.z;
            M[0][2] =-f.x;
            M[1][2] =-f.y;
            M[2][2] =-f.z;
            M[3][0] =-dot(s, eye);
            M[3][1] =-dot(u, eye);
            M[3][2] = dot(f, eye);
            return M;
        }

        glm::mat4 frustum(double l, double r, double b, double t, double n, double f){
            glm::mat4 M = glm::mat4(1.0f);
            M[0][0] = 2*n/(r-l);
            M[1][1] = 2*n/(t-b);
            M[2][0] = (r+l)/(r-l);
            M[2][1] = (t+b)/(t-b);
            M[2][2] = -(f+n)/(f-n);
            M[2][3] = -1;
            M[3][2] = -2*f*n/(f-n);
            M[3][3] = 0;
            return M;
        }

        glm::mat4 translate3D(glm::vec3 translateVector){
            glm::mat4 M = glm::mat4(1.0f);
            M[3][0] = translateVector.x;
            M[3][1] = translateVector.y;
            M[3][2] = translateVector.z;
            return M;
        }

        glm::mat4 scale3D(glm::vec3 scaleVector){
            glm::mat4 M = glm::mat4(1.0f);
            M[0][0] = scaleVector.x;
            M[1][1] = scaleVector.y;
            M[2][2] = scaleVector.z;
            return M;
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

        void setModelMatrix(glm::mat4 modelMatrix){
            this->modelMatrix = modelMatrix;
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
