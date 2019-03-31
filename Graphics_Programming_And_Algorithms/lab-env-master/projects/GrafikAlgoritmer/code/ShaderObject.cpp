//
// Created by nilsnils on 2018-04-10.
//

#include "config.h"
#include "ShaderObject.h"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>


ShaderObject::ShaderObject()
{
}


ShaderObject::~ShaderObject()
{
}

string ShaderObject::readFile(const char *filePath)																				
{
    string content;
    ifstream fileStream(filePath, ios::in);																			

    if (!fileStream.is_open())																						
    {
        cerr << "Could not read file " << filePath << ". File does not seem to exist." << std::endl;				
        return "";																									
    }

    string delimChar = "";
    while (!fileStream.eof())																						
    {
        getline(fileStream, delimChar);																				
        content.append(delimChar + "\n");																			
    }

    fileStream.close();																								
    return content;

}

GLuint ShaderObject::LoadShader(const char* vertex_path, const char* fragment_path)
{
    GLint result = GL_FALSE;
    GLint logLength;

    vShader = glCreateShader(GL_VERTEX_SHADER);																
    pShader = glCreateShader(GL_FRAGMENT_SHADER);

    //load shaders
    string vertShaderStr = readFile(vertex_path);																	
    string fragShaderStr = readFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();																
    const char *fragShaderSrc = fragShaderStr.c_str();

    //bind and compile vertex shader
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);																
    glCompileShader(vShader);

    //check if shader compiles/ works
    glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* buf = new GLchar[logLength];
        glGetShaderInfoLog(vShader, logLength, NULL, buf);
        printf("[SHADER COMPILE ERROR]: %s", buf);
        delete[] buf;
    }

    //bind and compile pixel/fragment shader
    glShaderSource(pShader, 1, &fragShaderSrc, NULL);
    glCompileShader(pShader);

    // debug pixel/fragment shader
    glGetShaderiv(pShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* buf = new GLchar[logLength];
        glGetShaderInfoLog(pShader, logLength, NULL, buf);
        printf("[SHADER COMPILE ERROR]: %s", buf);
        delete[] buf;
    }

    //create program and link shaders to it
    programID = glCreateProgram();																				
    glAttachShader(programID, vShader);																			
    glAttachShader(programID, pShader);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);																										
    if (logLength > 0)
    {
        GLchar* buf = new GLchar[logLength];
        glGetProgramInfoLog(programID, logLength, NULL, buf);
        printf("[PROGRAM LINK ERROR]: %s", buf);
        delete[] buf;
    }

    glDeleteShader(vShader);
    glDeleteShader(pShader);

    return programID;
}

void ShaderObject::startProgram(const char* vertex_path, const char* fragment_path)
{
    GLuint program = LoadShader(vertex_path, fragment_path);
    glUseProgram(program);
}

void ShaderObject::modifyUniformMatrix4(char* nameofMatrix4, float* floatarray)
{
    if (uniformMap.find(nameofMatrix4) == uniformMap.end())															
    {
        uniformMap[nameofMatrix4] = glGetUniformLocation(programID, nameofMatrix4);									
    }
    glUniformMatrix4fv(uniformMap.find(nameofMatrix4)->second, 1, GL_TRUE, floatarray);								
}

void ShaderObject::modifyUniformVector4(char* nameofVec4, float* floatarray)
{
    if (uniformMap.find(nameofVec4) == uniformMap.end())															
        uniformMap[nameofVec4] = glGetUniformLocation(programID, nameofVec4);										
    }
    glUniform4fv(uniformMap.find(nameofVec4)->second, 1, floatarray);												
}