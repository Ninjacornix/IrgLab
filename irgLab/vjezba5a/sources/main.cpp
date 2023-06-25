#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#include "Object.hpp"
#include "Render.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

using namespace std;

vector<Object> objects;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vjezba 5a", NULL, NULL);
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
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });


    string location = "/Users/ninjacornix/irglab/irgLab/vjezba5a/resources/kocka.obj";
    Object o = Object(location.c_str());
    objects.push_back(o);
    Render r = Render(objects);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Camera c = Camera(glm::vec3(3.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    vector<glm::mat4> matrices = c.getPerspectiveMatrix(800, 600);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        Shader s = r.getShader();
        s.setMat4("projection", matrices[1]);
        s.setMat4("view", matrices[0]);
        s.setMat4("model", matrices[2]);

        r.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
