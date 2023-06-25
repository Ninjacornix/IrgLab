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
    unsigned int VAO, VBO[3];
    vector<Vertex> points;
    vector<Vertex> approximation;
    vector<Vertex> interpolation;

    void compute_bezier(vector<int> &factors, int n)
    {
        int i, a = 1;

        for (i = 1; i <= n + 1; i++)
        {
            factors[i - 1] = a;
            a = a * (n - i + 1) / i;
        }
    }

public:
    Curve()
    {
        this->points = vector<Vertex>();
    }

    void addPoints(vector<glm::vec3> points)
    {
        for (auto p : points)
        {
            Vertex v;
            v.position = p;
            this->points.push_back(v);
        }
    }

    void addPoint(glm::vec3 point)
    {
        Vertex v;
        v.position = point;
        this->points.push_back(v);
        calculateApprox();
        calculateInterpolation();
    }

    void calculateApprox()
    {
        vector<Vertex> vertices;
        int divs = this->points.size() * 10;
        int n = this->points.size() - 1;
        double t, b;
        vector<int> factors(this->points.size(), 0);
        compute_bezier(factors, n);
        Vertex p;
        for (int i = 0; i <= divs; i++)
        {
            t = 1.0 / divs * i;
            p.position.x = 0;
            p.position.y = 0;
            p.position.z = 0;
            for (int j = 0; j <= n; j++)
            {
                if (j == 0)
                {
                    b = factors[j] * pow(1 - t, n);
                }
                else if (i == n)
                {
                    b = factors[j] * pow(t, n);
                }
                else
                {
                    b = factors[j] * pow(t, j) * pow(1 - t, n - j);
                }
                p.position.x += this->points[j].position.x * b;
                p.position.y += this->points[j].position.y * b;
                p.position.z += this->points[j].position.z * b;
            }
            vertices.push_back(p);
        }

        this->approximation = vertices;

    }

    void calculateInterpolation()
    {
        vector<glm::vec3> calcPoints;

        glm::mat4 potMatrix(1.0f);
        potMatrix[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        potMatrix[1] = glm::vec4(1.0f / 27.0f, 1.0f / 9.0f, 1.0f / 3.0f, 1.0f);
        potMatrix[2] = glm::vec4(8.0f / 27.0f, 4.0f / 9.0f, 2.0f / 3.0f, 1.0f);
        potMatrix[3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        glm::mat4 flipMat = glm::inverse(potMatrix);

        glm::mat4 polyDotsMatrix;

        for (int i = 0; i < 4; i++)
        {

            polyDotsMatrix[i] = glm::vec4(float(points[points.size() - 1 - i].position.x), float(points[points.size() - 1 - i].position.y), float(points[points.size() - 1 - i].position.z), 0.0f);
        }

        glm::mat4 KMat = polyDotsMatrix * flipMat;

        for (int i = 0; i <= this->points.size() * 10; i++)
        {

            float t = 1.0f / (this->points.size() * 10) * i;

            glm::vec3 point = glm::vec3(0.0, 0.0, 0.0);

            point.x = pow(t, 3) * KMat[0].x + pow(t, 2) * KMat[1].x + t * KMat[2].x + KMat[3].x;
            point.y = pow(t, 3) * KMat[0].y + pow(t, 2) * KMat[1].y + t * KMat[2].y + KMat[3].y;
            point.z = pow(t, 3) * KMat[0].z + pow(t, 2) * KMat[1].z + t * KMat[2].z + KMat[3].z;

            calcPoints.push_back(point);
        }

        for (glm::vec3 p : calcPoints)
        {

            if (p.x < 1 * pow(10, -5))
            {
                p.x = 0;
            }

            if (p.y < 1 * pow(10, -5))
            {
                p.y = 0;
            }

            if (p.z < 1 * pow(10, -5))
            {
                p.z = 0;
            }
        }

        this->interpolation = vector<Vertex>();
    }

    void draw_bezier()
    {
        glLineWidth(2);
        glGenBuffers(1, VBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex), &points[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(0);
        glDrawElements(GL_POINTS, points.size(), GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, approximation.size() * sizeof(Vertex), &approximation[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, approximation.size());

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, interpolation.size() * sizeof(Vertex), &interpolation[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, interpolation.size());

        glBindVertexArray(0);
    }

    vector<Vertex> getPoints()
    {
        return this->approximation;
    }
};
