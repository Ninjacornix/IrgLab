// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "FPSManager.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace std;

int width = 500, height = 500;

unsigned int VBO[4], VAO[3], EBO[2];

vector<float> vertices;
vector<float> colors;
vector<unsigned int> indices = {0, 1, 2};

vector<float> temp_dots;

int nums = 3;
int ind = 1;

int drawnums = 2;

float r = 0.5f;
float g = 0.5f;
float b = 0.5f;

void mousePos(GLFWwindow *window){
    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[2]);

    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * temp_dots.size(), &temp_dots[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void indicator(){
    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[3]);
    glGenBuffers(1, &EBO[1]);

    float vertices2[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, 0.95f, 0.0f,
        -0.95f, 0.95f, 0.0f,

        -0.95f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -0.95f, 0.95f, 0.0f
    };

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void checkUpdates(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);


    glBindVertexArray(0); 
}

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 oc;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   oc = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

Shader* loadShader(char* path, char* naziv) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;

	pathVert.clear();
	pathFrag.clear();

	pathVert.append("/Users/ninjacornix/irglab/irgLab/vjezba3/shaders/shader.vert");
	pathFrag.append("/Users/ninjacornix/irglab/irgLab/vjezba3/shaders/shader.frag");

	return new Shader(pathVert.c_str(), pathFrag.c_str());
}

//funkcija koja se poziva prilikom mijenjanja velicine prozora, moramo ju poveyati pomocu glfwSetFramebufferSizeCallback
void framebuffer_size_callback(GLFWwindow * window, int Width, int Height)
{
	width = Width;
	height = Height;

	glViewport(0, 0, width, height);
}

void cursorPosCallback(GLFWwindow *window, double x, double y) {
    float xpos, ypos;
    ypos = height - y;
    xpos = (x / width) * 2 - 1;
    ypos = (ypos / height) * 2 - 1;
    if(vertices.size() == 0){
        if(temp_dots.size() == 3){
            temp_dots.erase(temp_dots.begin(), temp_dots.begin() + 3);
        }
        temp_dots.push_back((float)xpos);
        temp_dots.push_back((float)ypos);
        temp_dots.push_back(0.0f);
    }else if(vertices.size() == 3){
        if(temp_dots.size() == 6){
            temp_dots.erase(temp_dots.begin() + 3, temp_dots.begin() + 6);
        }
        temp_dots.push_back((float)xpos);
        temp_dots.push_back((float)ypos);
        temp_dots.push_back(0.0f);
    } else if (vertices.size() == 6){
        if(temp_dots.size() == 9){
            temp_dots.erase(temp_dots.begin() + 6, temp_dots.begin() + 9);
        }
        temp_dots.push_back((float)xpos);
        temp_dots.push_back((float)ypos);
        temp_dots.push_back(0.0f);
        drawnums = 3;
    } else {
        temp_dots.clear();
        for (size_t i = vertices.size() - 6; i < vertices.size(); i++)
        {
            temp_dots.push_back(vertices[i]);
        }
        temp_dots.push_back((float)xpos);
        temp_dots.push_back((float)ypos);
        temp_dots.push_back(0.0f);
    }
    //print temp_dots
    for(int i = 0; i < temp_dots.size(); i++){
        std::cout << temp_dots[i] << " ";
        if((i + 1) % 3 == 0){
            std::cout << std::endl;
        }
    }
    cout << "-----------------" << endl;
};

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//get cursor position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
        ypos = height - ypos;
		//normalize between -1 and 1
		xpos = (xpos / width) * 2 - 1;
		ypos = (ypos / height) * 2 - 1;
		//add to vector
        vertices.push_back((float)xpos);
        vertices.push_back((float)ypos);
        vertices.push_back(0.0f);
        colors.push_back(r);
        colors.push_back(g);
        colors.push_back(b);

        if(vertices.size() > 11){
            for(int i = ind; i < ind + 3; i++){
                indices.push_back(i);
            }
            ind++;
            nums += 3;
        }

        for(int i = 0; i < vertices.size(); i++){
            std::cout << vertices[i] << " ";
            cout << colors[i] << " ";
            if((i + 1) % 3 == 0){
                std::cout << std::endl;
            }
        }
        
        cout << "-----------------" << endl;

        for(int i = 0; i < indices.size(); i++){
            std::cout << indices[i] << " ";
            if((i + 1) % 3 == 0){
                std::cout << std::endl;
            }
        }
	} else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		std::cout << "Desni klik" << std::endl;
	}
};

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS){
        if(r > 0.0f){
            r -= 0.01f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS){
        if(r < 1.0f){
            r += 0.01f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS){
        if(g > 0.0f){
            g -= 0.01f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS){
        if(g < 1.0f){
            g += 0.01f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS){
        if(b > 0.0f){
            b -= 0.01f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS){
        if(b < 1.0f){
            b += 0.01f;
        }
    }
    cout << "r: " << r << " g: " << g << " b: " << b << endl;
};

int main(int argc, char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyboardCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // -----------------------------------------------------------------
    indicator();
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


		if(vertices.size() > 8) {
			checkUpdates();
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, nums, GL_UNSIGNED_INT, 0);
		}

        glUniform4f(vertexColorLocation, r, g, b, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        mousePos(window);
        if(temp_dots.size() > 3){
            glUseProgram(shaderProgram);
            glBindVertexArray(tmp_VAO);
            glDrawArrays(GL_LINE_LOOP, 0, drawnums);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
