#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Transform.hpp"
#include "Shader.hpp"

#pragma once

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
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

    void setUP(){
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

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        glBindVertexArray(0);
    }
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VBO, EBO, VAO;
    Transform transform;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setUP();
        transform = Transform();
    }
    void draw(Shader &shader){
        unsigned int diffuse = 1;
        unsigned int specular = 1;
        unsigned int normal = 1;
        unsigned int height = 1;
        //cout << textures.size() << endl;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuse++);
            else if (name == "texture_specular")
                number = std::to_string(specular++);
            else if (name == "texture_normal")
                number = std::to_string(normal++);
            else if (name == "texture_height")
                number = std::to_string(height++);

            glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);

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
            v.position = transform.apply(v.position, center, maxDim);
        }
    }
};

