#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

#include "Object.hpp"
#include "Render.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Curve.hpp"

using namespace std;

vector<Object> objects;
vector<glm::vec3> points;
bool animation = false;
bool draw = false;
Curve curve = Curve();
Camera c = Camera(glm::vec3(2.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        c.moveForward(0.1f);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        c.moveForward(-0.1f);
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        c.moveRight(-0.1f);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        c.moveRight(0.1f);
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        c.moveUp(0.1f);
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        c.moveUp(-0.1f);
    }
    else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    {
        points.push_back(c.getEye());
        if (points.size() == 4)
        {
            curve.addPoints(points);
            draw = true;
        }
        else if (points.size() > 4)
        {
            curve.addPoint(c.getEye());
        }
        curve.calculateApprox();
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (draw)
        {
            if (animation)
            {
                animation = false;
            }
            else
            {
                animation = true;
            }
        }
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX = 400, lastY = 300;
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.005f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    c.rotate(xoffset, glm::vec3(0.0f, 1.0f, 0.0f));
    c.rotate(yoffset, glm::vec3(1.0f, 0.0f, 0.0f));
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Vjezba 5b", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });

    string location = "/Users/ninjacornix/irglab/irgLab/vjezba6/resources/sus_rotated.obj";
    Object o = Object(location.c_str());
    objects.push_back(o);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Render r = Render(objects);
    Render r1 = Render();

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    int i = 0;
    while (!glfwWindowShouldClose(window))
    {
        vector<glm::mat4> matrices = c.getPerspectiveMatrix();
        glm::vec3 eye = c.getEye();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // glDrawArrays(GL_TRIANGLES, 0, 36);

        Shader s = r.getShader();

        s.setMat4("projection", matrices[1]);
        s.setMat4("view", matrices[0]);
        s.setMat4("model", matrices[2]);
        s.setVec3("eye", eye);

        r.draw();
        
        if (animation)
        {
            Shader s1 = r1.getShader();

            s1.setMat4("projection", matrices[1]);
            s1.setMat4("view", matrices[0]);
            s1.setMat4("model", matrices[2]);
            s1.setVec3("eye", eye);


            vector<Vertex> vertices = curve.getPoints();
            if (i == 0)
            {
                c.setEye(vertices[0].position);
                i++;
            }

            if (i >= vertices.size())
            {
                i = 0;
                c.setEye(vertices[vertices.size() - 1].position);
                animation = false;
            }

            if (vertices[i].position.x - vertices[i - 1].position.x > 0.01f)
            {
                c.moveRight(1.0f);
            }
            else if (vertices[i].position.x - vertices[i - 1].position.x < -0.01f)
            {
                c.moveRight(-(1.0f));
            }

            if (vertices[i].position.y - vertices[i - 1].position.y > 0.01f)
            {
                c.moveUp(1.0f);
            }
            else if (vertices[i].position.y - vertices[i - 1].position.y < -0.01f)
            {
                c.moveUp(-(1.0f));
            }

            if (vertices[i].position.z - vertices[i - 1].position.z > 0.01f)
            {
                c.moveForward(1.0f);
            }
            else if (vertices[i].position.z - vertices[i - 1].position.z < -0.01f)
            {
                c.moveForward(-(1.0f));
            }
            //usleep(100000);
            i++;
            curve.draw_bezier();
        } else if (draw){
            Shader s1 = r1.getShader();

            s1.setMat4("projection", matrices[1]);
            s1.setMat4("view", matrices[0]);
            s1.setMat4("model", matrices[2]);
            s1.setVec3("eye", eye);
            curve.draw_bezier();
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
