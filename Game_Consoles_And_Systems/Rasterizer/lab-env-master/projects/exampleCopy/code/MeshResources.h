#pragma once
#include "core/app.h"
#include "render/window.h"
#include <iostream>
#include <vector>
#include "Matrix3D.h"

class MeshResources
{
public:

	struct Vertex
	{
		Vector3D Pos;
		Vector3D UV;
		Vector3D Norm;

		Vertex(const Vertex &obj)			//copy constructor
		{
			Pos = obj.Pos;
			Norm = obj.Norm;
			UV = obj.UV;
		}
		Vertex(Vector3D Pos, Vector3D UV, Vector3D Norm)
		{
			this->Pos = Pos;
			this->Norm = Norm;
			this->UV = UV;
		}
		Vertex()							//default constructor
		{

		}
	};

	static MeshResources* loadOBJfile(const char* filepath);
	static MeshResources* loadOBJfile_Rast(const char* filepath);
	bool draw();
	void setupVBO(Vertex* vertexbuffer, int numOfVertices);
	void setupIBO(GLuint* indexbuffer, int numOfIndices);
	MeshResources();
	~MeshResources();
	void CreateQuad();
	static MeshResources* CreateCube();
	std::vector<Vertex> getVertBuffer();
	std::vector<int> getIndBuffer();

private:
	std::vector<int> IndBuff;
	std::vector<Vertex> vertBuff;
	GLuint Vertex_buffer;
	GLuint Index_buffer;
	int indexCount;
	int* IntBuffer;
	
	
};
