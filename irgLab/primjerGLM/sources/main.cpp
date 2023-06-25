// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/ext.hpp"


// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>

const int mWidth = 1280;
const int mHeight = 800;


int main(int argc, char * argv[]) {

	//zbroj vektora
	glm::vec3 v1 = glm::vec3(2, 3, -4) + glm::vec3(-1, 4, -3);
	std::cout << "v1= " << glm::to_string(v1) << std::endl;
	
	//dot umnozak dva vektora
	glm::vec3 s = v1 * glm::vec3(-1, 4, -3);
	std::cout << "s= " << glm::to_string(s) << std::endl;

	//vektorski umnozak dva vektora
	glm::vec3 v2 = glm::cross(v1, glm::vec3(2, 2, 4));
	std::cout << "v2= " << glm::to_string(v2) << std::endl;

	//normaliziranje vektora
	glm::vec3 v3 = glm::normalize(v2);
	std::cout << "v3= " << glm::to_string(v3) << std::endl;

	//vektor u suprotnom smjeru
	glm::vec3 v4 = -v2;
	std::cout << "v4= " << glm::to_string(v4) << std::endl;

	//zbroj dvije matrice
	glm::mat3 m1 = glm::mat3(1, 2, 3, 2, 1, 3, 4, 5, 1) + glm::mat3(-1, 2, -3, 5, -2, 7, -4, -1, 3);
	std::cout << "m1= " << glm::to_string(m1) << std::endl;

	//umnozak matrica i transponiranje matrice
	glm::mat3 m2 = glm::mat3(1, 2, 3, 2, 1, 3, 4, 5, 1) * glm::transpose(glm::mat3(-1, 2, -3, 5, -2, 7, -4, -1, 3));
	std::cout << "m2= " << glm::to_string(m2) << std::endl;

	//umnožak dva inverza matrice
	glm::mat3 m3 = glm::inverse(glm::mat3(-24, 18, 5, 20, -15, -4, -5, 4, 1)) * glm::inverse(glm::mat3(1, 2, 3, 0, 1, 4, 5, 6, 0));
	std::cout << "m3= " << glm::to_string(m3) << std::endl;


	//rjesavanje linearnih jednadzbi 
	//////////////////////////////////////////////////////////////
	glm::mat3 jednadzba;
	glm::vec3 vektorJednadzbe;

	std::cout << "ucitaj parametre jednadzbi:";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			std::cin >> jednadzba[j][i];
		std::cin >> vektorJednadzbe[i];
	}
	glm::vec3 rjesenje = glm::inverse(jednadzba) * vektorJednadzbe;
	std::cout << "rjesenje jednadzbe= " << glm::to_string(rjesenje) << std::endl;

	//izracun baricentricne koordinate
	/////////////////////////////////////////////////////////////
	glm::mat3 jednadzba2;
	glm::vec3 vektorJednadzbe2;
	std::cout << "ucitaj koordinate tocki:";
	for (int i = 0; i < 3; i++) {
		std::cout << "tocka " << i << ":";
		for (int j = 0; j < 3; j++)
			std::cin >> jednadzba2[i][j];
	}

	std::cout << "ucitaj tocku za testiranje:";
	for (int i = 0; i < 3; i++)
		std::cin >> vektorJednadzbe2[i];

	glm::vec3 rjesenje2 = glm::inverse(jednadzba2) * vektorJednadzbe2;
	std::cout << "rjesenje jednadzbe= " << glm::to_string(rjesenje2) << std::endl;


    return EXIT_SUCCESS;
}
