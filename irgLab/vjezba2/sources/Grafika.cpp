#include "Grafika.h"

#include <cmath>
#include <iostream>
#include <string>


void (*Grafika::mouse_callback_user)(int, int, int) = 0;
glm::vec2 Grafika::cursorPosition(0, 0);
GLFWwindow *Grafika::window = 0;

void Grafika::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		(*mouse_callback_user)(cursorPosition.x / 10, cursorPosition.y / 10, 0);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		(*mouse_callback_user)(cursorPosition.x / 10, cursorPosition.y / 10, 1);

}

void Grafika::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	cursorPosition = glm::vec2(xpos, ypos);
}

int Grafika::loadGlfw()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(width * 10, height * 10, "Raster", nullptr, nullptr);

	// Check for Valid Context
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
}

int Grafika::loadRasterShader(char* path)
{
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;

	/* pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	if (pathFrag[pathFrag.size() - 1] == '/') {
		pathVert.append("shaders/shader.vert");
		pathFrag.append("shaders/shader.frag");
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\') {
		pathVert.append("shaders\\shader.vert");
		pathFrag.append("shaders\\shader.frag");
	}
	else {
		std::cerr << "nepoznat format pozicije shadera";
		exit(1);
	} */

	pathVert.clear();
	pathFrag.clear();

	pathVert.append("/Users/ninjacornix/irglab/irgLab/vjezba1/shaders/shader.vert");
	pathFrag.append("/Users/ninjacornix/irglab/irgLab/vjezba1/shaders/shader.frag");

	rasterShader = new Shader(pathVert.c_str(), pathFrag.c_str());
}



Grafika::Grafika(int Width, int Height, glm::vec3 ClearColor, char *path):
	width(Width),
	height(Height),
	clearColor(ClearColor)
{
	raster = new float[Width * Height *3];
	for (int i =0; i < height; i++)
		for (int j = 0; j < width; j++) {
			raster[i*width * 3 + j * 3] = clearColor.x;
			raster[i*width * 3 + j * 3 + 1] = clearColor.y;
			raster[i*width * 3 + j * 3 + 2] = clearColor.z;
		}

	loadGlfw();

	gladLoadGL();
	
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);

	loadRasterShader(path);

	

	glUniform1i(glGetUniformLocation(rasterShader->ID, "texture1"), 0);

	glGenTextures(1, &rasterID);
	glBindTexture(GL_TEXTURE_2D, rasterID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, (const void*)raster);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rasterVertices), rasterVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}

Grafika::~Grafika()
{
	delete[] raster;
	delete rasterShader;
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
}

int Grafika::osvijetliFragment(int x, int y) {
	return osvijetliFragment(x, y, glm::vec3(1, 1, 1));
}

int Grafika::osvijetliFragment(int x, int y, glm::vec3 boja){
	if (x >= 0 && x < width || y >= 0 && y < height) {
		raster[y*width * 3 + x * 3] = boja.x;
		raster[y*width * 3 + x * 3 + 1] = boja.y;
		raster[y*width * 3 + x * 3 + 2] = boja.z;
		return 0;
		
	}
	else {
		std::cerr << "ERROR: pokusaj osvjetljavanja izvan podrucja";
		return -1;
	}
}

	

void Grafika::pobrisiProzor()
{
	std::fill_n(raster, width*height * 3, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}



void Grafika::iscrtajRaster()
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//glBindTexture(GL_TEXTURE_2D, rasterID);
	glUseProgram(rasterShader->ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rasterID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, (const void*)raster);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);


	glfwSwapBuffers(window);
	glfwPollEvents();

}

bool Grafika::trebaZatvoriti()
{
	return glfwWindowShouldClose(window) == false;
}

int Grafika::registrirajFunkcijuZaKlikMisa(void(*Mouse_callback_user)(int, int, int))
{
	
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	mouse_callback_user = Mouse_callback_user;
	
	return 0;
}

