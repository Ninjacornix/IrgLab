#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once

namespace Transform
{
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
}
