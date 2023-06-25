#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <cstdint>

#include <fstream>
#include <sstream>

#include "Shader.h"


class Grafika
{
private:


	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void(*mouse_callback_user)(int, int, int);

	int loadGlfw();
	int loadRasterShader(char* path);


	int width;
	int height;

	glm::vec3 clearColor;


	static GLFWwindow* window;

	float rasterVertices[20] = { 
	//  verticesCoord	textureCoord
		-1, -1, 0,		0, 0,
		 1, -1, 0,		1, 0,
		-1,  1, 0,		0, 1,
		 1,  1, 0,		1, 1 };


	float *raster;
	unsigned int rasterID;
	unsigned int VAO;
	unsigned int VBO;

	Shader *rasterShader;

	

public:
	//konstruktor sa sirinom i visinom prozora, bojom kojom se brise prozor i put do programa
	Grafika(int width, int height, glm::vec3 clearColor, char *path); 
	//destruktor koji oslobodi memoriju i unisti kontekst OpenGL-a
	~Grafika();
	// pozicija kursora
	static glm::vec2 cursorPosition;

	//osvjetli fragment rastera na koordinatama x i y, gdje je ishodiste u donjem lijevom kutu
	int osvijetliFragment(int x, int y, glm::vec3 boja);
	int osvijetliFragment(int x, int y);

	void pobrisiProzor();

	//pozovi funkcije u OpenGL-u koje iscrtaju raster
	void iscrtajRaster();

	bool trebaZatvoriti();

	static int registrirajFunkcijuZaKlikMisa(void(*mouse_callback_user)(int, int, int));

};

