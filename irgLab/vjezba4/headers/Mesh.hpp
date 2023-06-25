#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Transform.hpp"

#pragma once

using namespace std;

struct Vertex {
    glm::vec3 position;
};

class Mesh
{
private:

    glm::vec3 getMin() const {
        glm::vec3 min = vertices[0].position;
        for (auto v : vertices) {
            if (v.position.x < min.x) min.x = v.position.x;
            if (v.position.y < min.y) min.y = v.position.y;
            if (v.position.z < min.z) min.z = v.position.z;
        }
        return min;
    }

    glm::vec3 getMax() const {
        glm::vec3 max = vertices[0].position;
        for (auto v : vertices) {
            if (v.position.x > max.x) max.x = v.position.x;
            if (v.position.y > max.y) max.y = v.position.y;
            if (v.position.z > max.z) max.z = v.position.z;
        }
        return max;
    }
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int VBO, EBO, VAO;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices){
        this->vertices = vertices;
        this->indices = indices;
    }
    void draw(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    vector<glm::vec3> getBoundingBox() const {
        vector<glm::vec3> boundingBox;
        boundingBox.push_back(getMin());
        boundingBox.push_back(getMax());
        return boundingBox;
    }

    glm::vec3 getCenter() const {
        glm::vec3 min = getMin();
        glm::vec3 max = getMax();
        return glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
    }

    float maxDimension() const {
        glm::vec3 min = getMin();
        glm::vec3 maX = getMax();
        float x = maX.x - min.x;
        float y = maX.y - min.y;
        float z = maX.z - min.z;
        return max(max(x, y), z);
    }

    void applyTransform() {
        glm::vec3 center = getCenter();
        float maxDim = 2 / maxDimension();
        for (auto &v : vertices) {
            v.position = Transform::apply(v.position, center, maxDim);
        }
    }
};

