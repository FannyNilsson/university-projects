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
	while (!fileStream.eof())																						//loop until End Of File
	{
		getline(fileStream, delimChar);																				// collect each line in the file
		content.append(delimChar + "\n");																			//save each line in a string (content) an then carriage return to not get everything in one line
	}

	fileStream.close();																								//close stream when done
	return content;
							
}

GLuint ShaderObject::LoadShader(const char* vertex_path, const char* fragment_path)
{
	GLint result = GL_FALSE;
	GLint logLength;

	vShader = glCreateShader(GL_VERTEX_SHADER);																		//create empty shader object of type vertex shader
	pShader = glCreateShader(GL_FRAGMENT_SHADER);

	//läser in våra shaders
	string vertShaderStr = readFile(vertex_path);																	
	string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();																//cast strings to const char pekare using c_str().
	const char *fragShaderSrc = fragShaderStr.c_str();

	//binder och kompilerar vertex shadern
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);																//bind the shader with the code from the file det är här vi binder shadern med koden från filen
	glCompileShader(vShader);

	// vi kollar huruvida shadern kompilerar, om den fungerar osv
	glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar* buf = new GLchar[logLength];
		glGetShaderInfoLog(vShader, logLength, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	//binder och kompilerar pixel/fragment shadern
	glShaderSource(pShader, 1, &fragShaderSrc, NULL);
	glCompileShader(pShader);

	// felsöker pixel/fragment shadern
	glGetShaderiv(pShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar* buf = new GLchar[logLength];
		glGetShaderInfoLog(pShader, logLength, NULL, buf);
		printf("[SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	//skapar programmet och länkar shaders till detta
	programID = glCreateProgram();																					
	glAttachShader(programID, vShader);																				//attach shaders to program
	glAttachShader(programID, pShader);
	glLinkProgram(programID);	

	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);														//link the program
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
	if (uniformMap.find(nameofMatrix4) == uniformMap.end())															//check if matrix is in the map (Map returns end() if the object does not exist in the map)
	{
		uniformMap[nameofMatrix4] = glGetUniformLocation(programID, nameofMatrix4);									//since the matrix is not found in the map, it is added.
	}
	glUniformMatrix4fv(uniformMap.find(nameofMatrix4)->second, 1, GL_TRUE, floatarray);								//collect second from the map and gives it the value of floatarray
}

void ShaderObject::modifyUniformVector4(char* nameofVec4, float* floatarray)
{
	if (uniformMap.find(nameofVec4) == uniformMap.end())															
	{
		uniformMap[nameofVec4] = glGetUniformLocation(programID, nameofVec4);										
	}
	glUniform4fv(uniformMap.find(nameofVec4)->second, 1, floatarray);												
}