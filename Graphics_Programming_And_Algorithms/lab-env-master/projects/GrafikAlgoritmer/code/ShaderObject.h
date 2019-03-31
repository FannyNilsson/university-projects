#pragma once
#include "core/app.h"
#include "render/window.h"
#include <iostream>
#include <map>

using namespace std;

class ShaderObject
{
private:
    GLuint programID;
    GLuint vShader;
    GLuint pShader;

    map<char*, GLuint> uniformMap;												//en map som lagrar mina uniforms och dess platser

public:
    void modifyUniformVector4(char* nameofVec4, float * floatarray);
    void modifyUniformMatrix4(char* nameofMatrix4, float * floatarray);
    void startProgram(const char* vertex_path, const char* fragment_path);
    string readFile(const char *filePath);
    GLuint LoadShader(const char* vertex_path, const char* fragment_path);

    ShaderObject();
    ~ShaderObject();
};