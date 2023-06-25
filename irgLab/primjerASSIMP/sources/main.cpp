// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Standard Headers
#include <iostream>
#include <cstdlib>

const int mWidth = 1280;
const int mHeight = 800;


int main(int argc, char * argv[]) {

	Assimp::Importer importer;

	std::string path(argv[0]);
	std::string dirPath(path, 0, path.find_last_of("\\/"));
	std::string resPath(dirPath);
	resPath.append("\\resources"); //za linux pretvoriti u forwardslash
	std::string objPath(resPath);
	objPath.append("\\glava\\glava.obj"); //za linux pretvoriti u forwardslash

	const aiScene* scene = importer.ReadFile(objPath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		std::cerr << importer.GetErrorString();
		return false;
	}

	if (scene->HasMeshes()) {

		aiMesh * mesh = scene->mMeshes[0];

		std::cout << "ucitana poligonalna mreza" << std::endl;
		
		std::cout << "prvih 10 tocaka:" << std::endl;
		//popis svih tocaka u modelu s x, y, z koordinatama
		for (int i = 0; i < 10 && i < mesh->mNumVertices; i++)
			std::cout << mesh->mVertices[i].x << " " << mesh->mVertices[i].y << " " << mesh->mVertices[i].z << std::endl;
		
		std::cout << "prvih 10 uv koordinata na teksturi" << std::endl;
		//popis svih koordinata u texturi u modelu s u,v koordinatama 
		for (int i = 0; i < 10 && i < mesh->mNumVertices; i++)
			//moze biti vise kanala za koordinate teksture, najcesce je samo jedan zato [0]
			std::cout << mesh->mTextureCoords[0][i].x << " " << mesh->mTextureCoords[0][i].y << std::endl;


		std::cout << "prvih 10 poligona:" << std::endl;

		//svaki poligon se sastoji od 3 ili vise tocki. 
		for (int i = 0; i < 10 && i < mesh->mNumFaces; i++) {
			
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				std::cout << mesh->mFaces[i].mIndices[j] << " ";

			std::cout << "-> " << std::endl;
			//Do koordinata tih tocaka se dolazi preko prijasnjeg popisa.

			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				int vertex = mesh->mFaces[i].mIndices[j];
				std::cout << "   coordinates    xyz  "<< mesh->mVertices[vertex].x << " " << mesh->mVertices[vertex].y << " " << mesh->mVertices[vertex].z << std::endl;
			}

			std::cout << std::endl;

			//uz svaku tocku na poligonu je pridruzena i uv koordinata na teksturi preko istog indeksa

			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				int vertex = mesh->mFaces[i].mIndices[j];
				
				std::cout << "   texture         uv  " << mesh->mTextureCoords[0][vertex].x << " " << mesh->mTextureCoords[0][vertex].y << std::endl;
			}
			
			//svaka tocka ima i pridruzene normale preko istog indeksa

			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				int vertex = mesh->mFaces[i].mIndices[j];

				std::cout << "   normals        xyz  " << mesh->mNormals[vertex].x << " " << mesh->mNormals[vertex].y <<  " " << mesh->mNormals[vertex].z << std::endl;
			}

			std::cout << std::endl;
		}


	}

	//ucitavanje materijala unutar obj datoteke

	if (scene->HasMaterials()) {
		std::cout << "ucitana svojstva materijala" << std::endl;

		for (int i = 0; i < scene->mNumMaterials; i++) {
			std::cout << "materijal[" << i << "]:\n";

			aiString naziv;

			std::cout << "naziv: ";
			scene->mMaterials[i]->Get(AI_MATKEY_NAME, naziv);
			std::cout << naziv.C_Str() << std::endl;

			std::cout << "pozicija teksture: ";
		
			aiString texturePosition;
			int width, height, nrChannels;
			unsigned char *data;
			if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
				std::cout << texturePosition.C_Str() << std::endl;
				std::string texPath(resPath);
				
				
				texPath.append("\\glava\\");
				texPath.append(texturePosition.C_Str());
				data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
			}



			glm::vec3 ambientColor;
			aiColor3D ambientK, diffuseK, specularK, reflectiveK, emissiveK;

			std::cout << "ambient: ";
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
			std::cout << ambientK.r << " " << ambientK.g << " " << ambientK.b << std::endl;

			std::cout << "diffuse: ";
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
			std::cout << diffuseK.r << " " << diffuseK.g << " " << diffuseK.b << std::endl;

			std::cout << "specular: ";
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);
			std::cout << specularK.r << " " << specularK.g << " " << specularK.b << std::endl;
			

			std::cout << "emissive: ";
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveK);
			std::cout << emissiveK.r << " " << emissiveK.g << " " << emissiveK.b << std::endl;

			std::cout << "- - - - - - - - - - - - -" <<std::endl;


			std::cout << "____________________________________________________" << std::endl;
		}

	}


}
