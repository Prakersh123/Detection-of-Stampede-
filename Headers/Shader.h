#ifndef Shader_H
#define Shader_H

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;
class Shader
{
    public:
        unsigned int ID;
        Shader(const char* vertexPath,const char* fragmentPath);
        void use();
        void setBool(const string &name,bool value) const;
        void setInt(const string &name,int value) const;
        void setFloat(const string &name,float value) const;

};

Shader:: Shader(const char* vertexPath,const char* fragmentPath)
{
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;

        vShaderStream<<vShaderFile.rdbuf();
        fShaderStream<<fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();
        vertexCode= vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::Shader::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
      const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
unsigned int vertex, fragment;
int success;
char infoLog[512];
   
// vertex Shader
vertex = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertex, 1, &vShaderCode, NULL);
glCompileShader(vertex);
// print compile errors if any
glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::Shader::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
};
fragment = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragment,1,&fShaderCode,NULL);
glCompileShader(fragment);
glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::Shader::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
};
ID = glCreateProgram();
glAttachShader(ID, vertex);
glAttachShader(ID, fragment);
glLinkProgram(ID);
// print linking errors if any
glGetProgramiv(ID, GL_LINK_STATUS, &success);
if(!success)
{
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::Shader::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
}
  
// delete the shaders as they're linked into our program now and no longer necessary
glDeleteShader(vertex);
glDeleteShader(fragment);



}

void Shader :: use()
{
    glUseProgram(ID);
}
void Shader:: setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader:: setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader:: setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 
/* code */
#endif 
