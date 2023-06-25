#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#pragma once

class Shader
{
private:
    
public:
    unsigned int ID;
    Shader(const char * vertexPath, const char * fragmentPath){
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;
        vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        try
        {
        
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
        
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
        
            vShaderFile.close();
            fShaderFile.close();
        
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
        }
        catch (ifstream::failure e)
        {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
    
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
    
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
    
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED \n" << infoLog << endl;
        };
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED \n" << infoLog << endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED \n" << infoLog << endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader(){
        ID = 0;
    }

    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath){
        string vertexCode;
        string fragmentCode;
        string geometryCode;
        ifstream vShaderFile;
        ifstream fShaderFile;
        ifstream gShaderFile;

        vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
        gShaderFile.exceptions (ifstream::failbit | ifstream::badbit);

        try
        {
        
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            gShaderFile.open(geometryPath);
            stringstream vShaderStream, fShaderStream, gShaderStream;
        
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            gShaderStream << gShaderFile.rdbuf();	
        
            vShaderFile.close();
            fShaderFile.close();
            gShaderFile.close();
        
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();	
            geometryCode = gShaderStream.str();		
        }
        catch (ifstream::failure e)
        {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        const char * gShaderCode = geometryCode.c_str();

        unsigned int vertex, fragment, geometry;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED \n" << infoLog << endl;
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED \n" << infoLog << endl;
        }

        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);

        glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(geometry, 512, NULL, infoLog);
            cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED \n" << infoLog << endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glAttachShader(ID, geometry);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED \n" << infoLog << endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteShader(geometry);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setVec3(const std::string &name, const glm::vec3 &vec) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    void setFloat(const std::string &name, const float &f) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), f);
    }

    void use(){
        glUseProgram(ID);
    }
};