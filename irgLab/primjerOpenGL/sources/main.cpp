// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"


// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>

int width = 900, height = 600;


Shader* loadShader(char* path, char* naziv) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;

	//malo je nespretno napravljeno jer ne koristimo biblioteku iz c++17, a treba podrzati i windows i linux

	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	if (pathFrag[pathFrag.size() - 1] == '/') {
		pathVert.append("shaders/");
		pathFrag.append("shaders/");
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\') {
		pathVert.append("shaders\\");
		pathFrag.append("shaders\\");
	}
	else {
		std::cerr << "nepoznat format pozicije shadera";
		exit(1);
	}

	pathVert.append(naziv);
	pathVert.append(".vert");
	pathFrag.append(naziv);
	pathFrag.append(".frag");

	return new Shader(pathVert.c_str(), pathFrag.c_str());
}


void framebuffer_size_callback(GLFWwindow * window, int Width, int Height)
{
	width = Width;
	height = Height;
}
  

int main(int argc, char * argv[]) {
	
	//ako se ne pokrece!!
	//primjer 4b koristi funkcionalnosti iz novijih verzija OpenGL, pa ako ga zakomentirate mozda popravi vas problem
	
	GLFWwindow* window;


	std::cout << argv[0] << std::endl;

	glfwInit();

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	window = glfwCreateWindow(width, height, "neindeksirano/indeksirano/instancirano", nullptr, nullptr);

	// Check for Valid Context
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora

	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glClearColor(0.15, 0.1, 0.1, 1);

	//generiranje buffera
	GLuint VAO[4];
	GLuint VBO[5];
	GLuint EBO;

	glGenVertexArrays(4, VAO);
	glGenBuffers(5, VBO);
	glGenBuffers(1, &EBO);
	
	Shader *sjencar[4];
	
	/*********************************************************************************************/
	// svaki iduci primjer prvo definira koji su sve podaci potrebni na grafickoj kartici, a potom
	// se unutar petlje iscrtava
	/*********************************************************************************************/
	//primjer 1
	//vrhovi kroz buffer, a boja kroz uniformnu varijablu

	sjencar[0] = loadShader(argv[0], "shader0");

	GLint lokacijaUniformVarijable = glGetUniformLocation(sjencar[0]->ID, "u_color");

	float trokutKoordinate[9] = {
		//  koordinate
			-1, -1, 0,
			 1, -1, 0,
			0,  1, 0 };
	
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trokutKoordinate), trokutKoordinate, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	/*********************************************************************************************/
	//primjer 2
	//interlaced vrhovi i boje kroz buffer

	sjencar[1] = loadShader(argv[0], "shader1");

	float obojaniTrokut[18] = {
		//  koordinate    boje
			-1, -1, 0,    1, 0, 0,
			 1, -1, 0,    0, 1, 0,
			0,  1, 0,     0, 0, 1 };

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obojaniTrokut), obojaniTrokut, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)((3 * sizeof(float))));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	/*********************************************************************************************/
	//primjer 3
	//indeksirani vrhovi i boje u odvojenim poljima. Koristimo isti sjencar jer je raspakiravanje indeksa na grafickoj skriveno od nas u koracima prije sjencara vrhova.

	float indeksiraniVrhovi[18] = {
			-1,  -1, 0,
			 1,  -1, 0,
			 0,   1, 0,
			-0.4, -0.1, 0,
			 0.4, -0.1, 0,
			 0,  -0.9, 0
	};
	float indeksiraneBoje[18] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		0, 1, 1,
		1, 0, 1,
		1, 1, 0
	};

	unsigned int indeksi[12] = {
		0, 5, 3,
		3, 5, 4,
		5, 1, 4,
		3, 4, 2
	};

	glBindVertexArray(VAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indeksiraniVrhovi), indeksiraniVrhovi, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indeksiraneBoje), indeksiraneBoje, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeksi), (void*)(&indeksi[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);

	/*********************************************************************************************/
	//primjer 4a
	//instanciranje istog "objekta" preko uniforme varijable, u odnosu na primjer 3 promijenio se samo shader i uvela dodatna uniform varijabla
	sjencar[2] = loadShader(argv[0], "shader2");

	GLint lokacijaUniformVarijable2 = glGetUniformLocation(sjencar[2]->ID, "tMatrica");

	//izrada polja transformacija koji  postavljaju objekte u mrezu 4x4
	glm::mat4 jedinicna = glm::mat4(1);
	glm::mat4 skaliranje = glm::scale(jedinicna, glm::vec3(0.25, 0.25, 0.25));
	glm::mat4 poljeTransformacija[16];
	int brojac = 0;
	for (float i = -1; i < 1; i+=0.5) {
		for (float j = -1; j < 1; j+=0.5) {
			poljeTransformacija[brojac++] =  glm::translate(jedinicna, glm::vec3(j+0.25, i+0.25 , 0)) * skaliranje;
		}
	}


	/*********************************************************************************************/
	//primjer 4b
	//instanciranje istog "objekta" preko dodatnog polja transformacija
	//kako smo vec napravili VBO i EBO u primjeru 3, mozemo ih reciklirati za ovaj primjer bez slanja na graficku karticu
	sjencar[3] = loadShader(argv[0], "shader3");

	glBindVertexArray(VAO[3]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(poljeTransformacija), poljeTransformacija, GL_STATIC_DRAW);

	//zbog ogranicenja sto mozemo poslati max 4 floata po layoutu moramo definirati vise pokazivaca
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	//definira da ce se podataci na ulazu sjencara vrhova za VertexAttribArray 2, 3, 4, i 5 osvjezavati samo na pocetku crtanja nove instance, a ne prilikom dohvacanja svakog vrha
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	glBindVertexArray(0);

	//glavna petlja programa
	while (glfwWindowShouldClose(window) == false) {


		glClear(GL_COLOR_BUFFER_BIT);

		
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		
		/********************************************************/
		//primjer 1
		glUseProgram(sjencar[0]->ID);
		glUniform3f(lokacijaUniformVarijable, 0.5, 1.0, 1.0);
	
		glViewport(0, height/2, width/3, height/2);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		/********************************************************/
		//primjer 2

		glUseProgram(sjencar[1]->ID);

		glViewport(width/3, height/2, width/3, height/2);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		/********************************************************/
		//primjer 3

		glUseProgram(sjencar[1]->ID);
		glViewport(2*width/3, height/2, width/3, height/2);

		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, sizeof(indeksi), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		/********************************************************/
		//primjer 4a
		//za svaku instancu objekta saljemo naredbu za iscrtavanje. podaci o modelu ostaju na grafickoj, mijenja se samo uniform varijabla.
		glUseProgram(sjencar[2]->ID);
		glViewport(0, 0, width / 3, height / 2);


		glBindVertexArray(VAO[2]);
		
		
		for (int i = 0; i < 16; i++) {
			glUniformMatrix4fv(lokacijaUniformVarijable2, 1, GL_FALSE, &poljeTransformacija[i][0][0]);
			glDrawElements(GL_TRIANGLES, sizeof(indeksi), GL_UNSIGNED_INT, 0);  
		}
		

		glBindVertexArray(0);


		/********************************************************/
		//primjer 4b
		//samo jednom pozivamo iscrtavanje za sve instance jer smo grafickoj poslali polje transformacija
		glUseProgram(sjencar[3]->ID);
		glViewport(width / 3,0, width / 3, height / 2);

		glBindVertexArray(VAO[3]);
		glDrawElementsInstanced(GL_TRIANGLES, sizeof(indeksi), GL_UNSIGNED_INT, 0, 16); 
		
		glBindVertexArray(0);
		
		 
		glfwSwapBuffers(window);
		glfwPollEvents();

	}   
	
	for (int i = 0; i < 4; i++)
		delete sjencar[i];
	
	glDeleteBuffers(5, VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(4, VAO);

	glfwTerminate();


    return EXIT_SUCCESS;
}
