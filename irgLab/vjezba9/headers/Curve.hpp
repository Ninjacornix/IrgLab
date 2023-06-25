#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#pragma once

using namespace std;

class Curve
{
private:
    unsigned int VAO, VBO;
    vector<Vertex> points;
    vector<Vertex> drawPoints;

    void compute_bezier(vector<int> &factors, int n){
        int i, a = 1;

        for (i = 1; i <= n + 1; i++){
            factors[i - 1] = a;
            a = a * (n - i + 1) / i;
        }
    }

public:
    Curve(){
        this->points = vector<Vertex>();
    }

    void addPoints(vector<glm::vec3> points){
        for (auto p : points){
            Vertex v;
            v.position = p;
            this->points.push_back(v);
        }
    }

    void addPoint(glm::vec3 point){
        Vertex v;
        v.position = point;
        this->points.push_back(v);
    }

    void draw_bezier(){
        vector<Vertex> vertices;
        int divs = this->points.size() * 100;
        int n = this->points.size() - 1;
        double t, b;
        vector<int> factors(this->points.size(), 0);
        compute_bezier(factors, n);
        Vertex p;
        for (int i = 0; i <= divs; i++){
            t = 1.0 / divs * i;
            p.position.x = 0;
            p.position.y = 0;
            p.position.z = 0;
            for (int j = 0; j <= n; j++){
                if (j == 0){
                    b = factors[j] * pow(1-t, n);
                } else if (i == n){
                    b = factors[j] * pow(t, n);
                } else {
                    b = factors[j] * pow(t, j) * pow(1-t, n-j);
                }
                p.position.x += this->points[j].position.x * b;
                p.position.y += this->points[j].position.y * b;
                p.position.z += this->points[j].position.z * b;
            }
            vertices.push_back(p);
        }

        this->drawPoints = vertices;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
        glBindVertexArray(0);
    }

    vector<Vertex> getPoints(){
        return this->points;
    }
};
