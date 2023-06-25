#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <string>

#include "Mesh.hpp"

#pragma once

using namespace std;

class Object
{
private:
    vector<Mesh> meshes;
    string dir;

    void load(string path){
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP:: " << import.GetErrorString() << endl;
            return;
        }
        dir = path.substr(0, path.find_last_of('/'));
        process(scene->mRootNode, scene);
    }

    void process(aiNode* node, const aiScene* scene){
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            meshes.push_back(meshProcess(mesh, vertices, indices));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            process(node->mChildren[i], scene);
        }
    }

    Mesh meshProcess(aiMesh* mesh, vector<Vertex>& vertices, vector<unsigned int>& indices){
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
            }
        } 
        return Mesh(vertices, indices);
    }
public:
    Object(const char* path){
        load(path);
    }
    void draw(){
        for (auto m : meshes) {
            m.applyTransform();
            m.draw();
        }
    }
};

