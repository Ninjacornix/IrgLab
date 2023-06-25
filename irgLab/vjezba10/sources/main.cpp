#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include<unistd.h>  

#include "Object.hpp"
#include "Render.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Curve.hpp"
#include "Light.hpp"
#include "Material.hpp"

using namespace std;

vector<Object> objects;
vector<glm::vec3> points;
/* bool animation = false;
bool draw = false; */
Camera c = Camera(glm::vec3(2.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
Curve curve = Curve();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS){
        c.moveForward(0.5f);
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS){
        c.moveForward(-0.5f);
    } else if (key == GLFW_KEY_A && action == GLFW_PRESS){
        c.moveRight(-0.5f);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS){
        c.moveRight(0.5f);
    } else if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        c.moveUp(0.5f);
    } else if (key == GLFW_KEY_E && action == GLFW_PRESS){
        c.moveUp(-0.5f);
    } /* else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS){
        points.push_back(c.getEye());
        if(points.size() == 4){
            curve.addPoints(points);
            draw = true;
        } else if (points.size() > 4){
            curve.addPoint(c.getEye());
        }
    } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        if (animation){
            animation = false;
        } else {
            animation = true;
        }
    } */
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vjezba 10", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 1280, 720);

    // framebuffer
    unsigned int FBO, depthMap;
    glGenFramebuffers(1, &FBO);
    glGenTextures(1, &depthMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //string location = "/Users/ninjacornix/irglab/irgLab/vjezba10/resources/glava/glava.obj";
    string location = "/Users/ninjacornix/irglab/irgLab/vjezba8b/resources/sus_rotated.obj";


    Object o = Object(location.c_str());
    Object front = Object(location.c_str());
    Object back = Object(location.c_str());
    Object left = Object(location.c_str());
    Object right = Object(location.c_str());
    Object frontLeft = Object(location.c_str());
    Object frontRight = Object(location.c_str());
    Object backLeft = Object(location.c_str());
    Object backRight = Object(location.c_str());
    o.scale();
    o.rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    front.setModelMatrix(glm::translate(front.getModelMatrix(), glm::vec3(0.0f, 0.0f, 1.0f)));
    front.rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    front.scale();

    back.setModelMatrix(glm::translate(back.getModelMatrix(), glm::vec3(0.0f, 0.0f, -1.0f)));
    back.rotate(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    back.scale();

    left.setModelMatrix(glm::translate(left.getModelMatrix(), glm::vec3(-1.0f, 0.0f, 0.0f)));
    left.rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    left.scale();

    right.setModelMatrix(glm::translate(right.getModelMatrix(), glm::vec3(1.0f, 0.0f, 0.0f)));
    right.rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    right.scale();

    frontLeft.setModelMatrix(glm::translate(frontLeft.getModelMatrix(), glm::vec3(-0.75f, 0.0f, 0.75f)));
    frontLeft.rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    frontLeft.scale();

    frontRight.setModelMatrix(glm::translate(frontRight.getModelMatrix(), glm::vec3(0.75f, 0.0f, 0.75f)));
    frontRight.rotate(glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    frontRight.scale();

    backLeft.setModelMatrix(glm::translate(backLeft.getModelMatrix(), glm::vec3(-0.75f, 0.0f, -0.75f)));
    backLeft.rotate(glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    backLeft.scale();

    backRight.setModelMatrix(glm::translate(backRight.getModelMatrix(), glm::vec3(0.75f, 0.0f, -0.75f)));
    backRight.rotate(glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    backRight.scale();

    objects.push_back(o);
    objects.push_back(front);
    objects.push_back(back);
    objects.push_back(left);
    objects.push_back(right);
    objects.push_back(frontLeft);
    objects.push_back(frontRight);
    objects.push_back(backLeft);
    objects.push_back(backRight);
    
    string vertexSource = "/Users/ninjacornix/irglab/irgLab/vjezba10/shaders/shader.vert";
    string fragmentSource = "/Users/ninjacornix/irglab/irgLab/vjezba10/shaders/shader.frag";
    string geometrySource = "/Users/ninjacornix/irglab/irgLab/vjezba10/shaders/shader.geom";
    Render faces = Render(objects, vertexSource, fragmentSource, geometrySource);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    int i = 0;

    string depthVertex = "/Users/ninjacornix/irglab/irgLab/vjezba10/shaders/depth.vert";
    string depthFragment = "/Users/ninjacornix/irglab/irgLab/vjezba10/shaders/depth.frag";
    //string depthGeometry = "/Users/ninjacornix/irglab/irgLab/vjezba10/shaders/depth.geom";

    Shader s = faces.getShader();
    s.use();
    s.setInt("texture_diffuse1", 0);
    s.setInt("shadowMap", 1);
    Shader depth = Shader(depthVertex.c_str(), depthFragment.c_str());

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        objects[0].rotate(glm::radians(4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::vec3 eye = c.getEye();
        vector<glm::mat4> matrices = c.getPerspectiveMatrix();
        Light l = Light();
        Material m = Material();

        glm::vec3 lightPos = l.getLightPos();
        lightPos.z = sin(glfwGetTime()) * 2.0f;

        float near_plane = 1.0f;
        float far_plane = 10.0f;

        glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 0.0));
        
        depth.use();
        //glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        depth.setMat4("lightProjection", lightProjection);
        depth.setMat4("lightView", lightView);
        depth.setMat4("lightPos", glm::translate(glm::mat4(1.0f), lightPos));
        depth.setFloat("far_plane", far_plane);
        for (size_t i = 0; i < objects.size(); i++)
        {
            /* if(i == 1){
                glm::mat4 testing = glm::translate(objects[i].getModelMatrix(), glm::vec3(1.0f, 0.0f, 0.0f));
                depth.setMat4("model", testing);
            } else {
            } */
            depth.setMat4("model", objects[i].getModelMatrix());
            objects[i].draw(depth);
            //faces.drawWithCustomShader(depth);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        s.setInt("shadowMap", 1);
        s.setFloat("far_plane", far_plane);
        s.setMat4("projection", matrices[1]);
        s.setMat4("view", matrices[0]);
        s.setMat4("lightView", lightView);
        s.setMat4("lightProjection", lightProjection);
        s.setVec3("light.ambient", l.getLightAmbient());
        s.setVec3("light.diffuse", l.getLightDiffuse());
        s.setVec3("light.specular", l.getLightSpecular());
        s.setVec3("material.ambient", m.getAmbient());
        s.setVec3("material.diffuse", m.getDiffuse());
        s.setVec3("material.specular", m.getSpecular());
        s.setFloat("material.shininess", m.getShininess());
        s.setVec3("eye", eye);
        s.setVec3("viewPos", eye);
        for (size_t i = 0; i < objects.size(); i++)
        {
            /* if(i == 1){
                glm::mat4 testing = glm::translate(objects[i].getModelMatrix(), glm::vec3(1.0f, 0.0f, 0.0f));
                //testing = glm::rotate(testing, glm::radians(90.0f), glm::vec3(1.0f, 0.3f, 0.5f));
                s.setMat4("model", testing);
            } else {
            } */
            glm::mat4 model = objects[i].getModelMatrix();
            s.setMat4("model", model);
            objects[i].draw(s);
            //faces.drawWithCustomShader(s);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
