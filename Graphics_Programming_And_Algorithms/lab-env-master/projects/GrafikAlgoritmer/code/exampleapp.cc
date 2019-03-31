//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	skel.ParseXMLfile("Unit_Footman.constants");

	App::Open();
	this->window = new Display::Window;
    window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
        float speed = 0.01f;

        //handle key input - rotation of joints as well as movement
        switch (key) {
            //case GLFW_KEY_SPACE
            //	this->window->Close();
            case GLFW_KEY_W:                                        //if key == s set speed on y-axis
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    this->skel.JointList[0].translation[3][1] += speed;
                } else {
                    this->skel.JointList[0].translation[3][1] += 0;
                }
                break;

            case GLFW_KEY_A:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    this->skel.JointList[0].translation[3][0] += -speed;
                } else {
                    this->skel.JointList[0].translation[3][0] += 0;
                }
                break;
            case GLFW_KEY_S:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    this->skel.JointList[0].translation[3][1] += -speed;               //if key == s set speed on x-axis
                } else {
                    this->skel.JointList[0].translation[3][1] += 0;
                }

                break;
            case GLFW_KEY_D:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    this->skel.JointList[0].translation[3][0] += speed;
                } else {
                    this->skel.JointList[0].translation[3][0] += 0;
                }
                break;
            case GLFW_KEY_ESCAPE:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    this->window->Close();
                }
                break;


            case GLFW_KEY_1:
                if (action == GLFW_PRESS) {
                    if (skel.JointList[14].isRotating)
                        skel.JointList[14].isRotating = false;
                    else
                        skel.JointList[14].isRotating = true;
                }
                break;

            case GLFW_KEY_2:
                if (action == GLFW_PRESS) {
                    if(skel.JointList[18].isRotating)
                        skel.JointList[18].isRotating = false;
                    else
                        skel.JointList[18].isRotating = true;
                }
                break;

            case GLFW_KEY_3:
                if (action == GLFW_PRESS) {
                    if(skel.JointList[9].isRotating)
                        skel.JointList[9].isRotating = false;
                    else
                        skel.JointList[9].isRotating = true;
                }
                break;

            case GLFW_KEY_4:
                if (action == GLFW_PRESS) {
                    if(skel.JointList[2].isRotating)
                        skel.JointList[2].isRotating = false;
                    else
                        skel.JointList[2].isRotating = true;
                }
                break;

            case GLFW_KEY_5:
                if (action == GLFW_PRESS) {
                    if(skel.JointList[1].isRotating)
                        skel.JointList[1].isRotating = false;
                    else
                        skel.JointList[1].isRotating = true;
                }
                break;
            //set the animations
            case GLFW_KEY_Z:
                if (action == GLFW_PRESS) {
                    animNum = 1;
                }
                break;
            case GLFW_KEY_X:
                if (action == GLFW_PRESS) {
                    animNum = 2;
                }
                break;
            case GLFW_KEY_C:
                if (action == GLFW_PRESS) {
                    animNum = 3;
                }
                break;
            case GLFW_KEY_V:
                if (action == GLFW_PRESS) {
                    animNum = 4;
                }
                break;
            case GLFW_KEY_B:
                if (action == GLFW_PRESS) {
                    animNum = 5;
                }
                break;
            case GLFW_KEY_N:
                if (action == GLFW_PRESS) {
                    animNum = 6;
                }
                break;
            case GLFW_KEY_M:
                if (action == GLFW_PRESS) {
                    animNum = 7;
                }
                break;
        }
	});

	GLfloat buf[] =
	{
		-0.5f,	-0.5f,	-1,			// pos 0
		1,		0,		0,		1,	// color 0
		0,		0.5f,	-1,			// pos 1
		0,		1,		0,		1,	// color 0
		0.5f,	-0.5f,	-1,			// pos 2
		0,		0,		1,		1	// color 0
	};

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = std::strlen(ps);
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}

		// setup vbo
		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
    anim.Nax3Parser("Unit_Footman.nax3");

    //skin = new Skinning();
    //skin->NVX2Parser("Unit_Footman.nvx2");
    //skin->SetUpVertices();
    //skin->SetUpVBO();
    //skin->SetUpIBO();
    //skin->LoadDDS("Footman_Diffuse.dds", skeleton, 0);
    //skin->LoadDDS("Footman_Specular.dds", skeleton, 1);
    //skin->LoadDDS("Footman_Normal.dds", skeleton, 2);

    GLuint matrixID = glGetUniformLocation(program, "matrix");
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

        for(int i=0; i<skel.JointList.size();i++)
        {
            if(skel.JointList[i].isRotating == true)
            {
                skel.JointList[i].translation = skel.JointList[i].translation * Matrix3D::rotate_x(5.0f);
            }
        }

        anim.SetUpAnimation(&skel,animNum);
        skel.Draw();

		this->window->SwapBuffers();
	}
}

} // namespace Example