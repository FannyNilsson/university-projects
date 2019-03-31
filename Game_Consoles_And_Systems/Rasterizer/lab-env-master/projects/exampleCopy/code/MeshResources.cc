#include "config.h"
#include "MeshResources.h"
#include <cstring>



MeshResources::MeshResources()
{
}


MeshResources::~MeshResources()
{
	glDeleteBuffers(1, &Vertex_buffer);
	glDeleteBuffers(1, &Index_buffer);
}

MeshResources* MeshResources::CreateCube()
{
	Vertex buffer[] =
	{
		Vertex(Vector3D(-0.5f,	-0.5f,	-0.5f),				// pos 0
		Vector3D(0,		0,		0),					// UV 0
		Vector3D(0,		0,		0)),					// normal 0

		Vertex(Vector3D (-0.5f,	0.5f,	-0.5f),			// pos 1
		Vector3D(0,		1,	0),						// UV 1
		Vector3D(0,		0,		0)),					// normal 1

		Vertex(Vector3D(0.5f,	0.5f,	-0.5f),				// pos 2
		Vector3D(1,		1,	0),						// UV 2
		Vector3D(0,		0,		0)),					// normal 2

		Vertex(Vector3D(0.5f,	-0.5f,	-0.5f),				// pos 3
		Vector3D(1,		0, 0), 						// UV 3
		Vector3D(0,		0,		0)),					// normal 3

		Vertex(Vector3D (-0.5f,	-0.5f,	0.5f),			// pos 4
		Vector3D(0,		0, 0), 						// UV 4
		Vector3D(0,		0,		0)),					// normal 4

		Vertex(Vector3D (-0.5f,	0.5f,	0.5f),			// pos 5
		Vector3D(0,		1,	0),						// UV 5
		Vector3D(0,		0,		0)),					// normal 5

		Vertex(Vector3D(0.5f,	0.5f,	0.5f),				// pos 6
		Vector3D(1,		1,	0),						// UV 6
		Vector3D(0,		0,		0)),					// normal 6

		Vertex(Vector3D(0.5f,	-0.5f,	0.5f),				// pos 7
		Vector3D(1,		0,	0),						// UV 7
		Vector3D(0,		0,		0)),					// normal 7
	};

	MeshResources* mesh = new MeshResources();
	mesh->setupVBO(buffer, 8);

	GLuint Indices[] =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};
	mesh->setupIBO(Indices, 36);
	return mesh;
}

void MeshResources::CreateQuad()
{
	Vertex buffer[] =
	{
		Vertex(Vector3D(-0.5f,	-0.5f,	-1),					// pos 0
		Vector3D(0,	0, 0),										// uv 0
		Vector3D(0, 0, 0)),										//normal 0

		Vertex(Vector3D(-0.5f, 0.5f, -1),						// pos 1
		Vector3D(0,	1, 0),										// uv 1
		Vector3D(0, 0, 0)),										//normal 1

		Vertex(Vector3D(0.5f, 0.5f, -1),						// pos 2
		Vector3D(1,	1, 0),										// uv 2
		Vector3D(0, 0, 0)),										//normal 2

		Vertex(Vector3D(0.5f,	-0.5f,	-1),					// pos 3
		Vector3D(1,	0,	0),										// uv 3
		Vector3D(0, 0, 0)),										//normal 3

	};
	setupVBO(buffer, 4);


	GLuint Indices[] =
	{
		//triangle seen as "above/left part of rectangle"
		0, 2, 1,

		//triangle seen as "below/right part of rectangle"
		0, 3, 2,
	};
	setupIBO(Indices,6);
}

void MeshResources::setupVBO(Vertex* buffer, int numOfVertices)
{
	glGenBuffers(1, &Vertex_buffer);																//create buffer
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_buffer);													//activate buffer
	glBufferData(GL_ARRAY_BUFFER, numOfVertices*sizeof(Vertex), buffer, GL_STATIC_DRAW);			//create and initialize buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);																//deactivate buffer
	return;
}

void MeshResources::setupIBO(GLuint * indices, int count)											//uses index buffer as argument (works as a "pointer to the buffer")
{
	glGenBuffers(1, &Index_buffer);																	//create buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index_buffer);											//activate buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(int), indices, GL_STATIC_DRAW);				//"load data"
	this->indexCount = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
bool MeshResources::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_buffer);															//activate buffer
	glEnableVertexAttribArray(0);																			//enable  vertexattributes in shadern
	glEnableVertexAttribArray(1);																			
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);									//define an array with attribute data. 3 means how many elements that are going to be used. Size of vertex struct gives the offset NULL (means that we start in the beginning)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D)));			//same as above but start at the third element to continue were it left off
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D) * 2));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index_buffer);													//activate index buffer
	glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, NULL);	//renderar 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

MeshResources* MeshResources::loadOBJfile_Rast(const char* filepath)
{
	MeshResources* mesh = new MeshResources();
	std::vector<unsigned int> vIndices, uvIndices, normalIndices;
	std::vector<Vector3D> temp_vertices;
	std::vector<Vector3D> temp_uvs;
	std::vector<Vector3D> temp_normals;

	std::vector<GLuint> indBuffer;

	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		printf("Cannot open file! \n");
		return NULL;
	}

	//abbreviatons in a OBJ file
	//v is a vertex
	//vt is the texture coordinate of one vertex
	//vn is the normal of one vertex
	//f is a face
	while (1)
	{

		char lineHeader[128];							//assume the first word on line won't be longer than 128 characters
		int res = fscanf(file, "%s", lineHeader);		// first word in line read by using fscanf.
		if (res == EOF)									//stop at End Of File (EOF) and break loop
		{
			break;
		}

		if (strcmp(lineHeader, "v") == 0)											//if first "word" (string) is "v" it means that the rest of the data will be about vetices
		{
			Vector3D vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);			//save all vertices in Vector3D and save that in member variable temp_vertices
			temp_vertices.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0)										//same with uv
		{
			Vector3D uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			temp_uvs.push_back(uv);
		}

		else if (strcmp(lineHeader, "vn") == 0)										//same with normals	
		{
			Vector3D Normal;
			fscanf(file, "%f %f %f\n", &Normal[0], &Normal[1], &Normal[2]);
			temp_normals.push_back(Normal);
		}

		else if (strcmp(lineHeader, "f") == 0)										//same with faces
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			
			int readFace = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (readFace == 9)
			{
				Vertex v;												//create struct instance and save all vertices and corresponding pos, uv and normals
				v.Pos = temp_vertices[vertexIndex[0] - 1];				//vertex 1. (temp_vertices has all vertices saved. Using VertexIndex to get the correct vertex in the face
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertex 2
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertex 3
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);
			}

			else if(readFace == 12)										//if OBJ file contains quads - split them up in groups to read the file
			{
				Vertex v;
				v.Pos = temp_vertices[vertexIndex[0] - 1];				//vertex 0
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertex 1
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertex 2
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);				//first triangle added to vertBuffer (0,1,2)

				v.Pos = temp_vertices[vertexIndex[0] - 1];				//same thing but second triangle
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertex 2
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[3] - 1];				//vertex 4
				v.Norm = temp_normals[normalIndex[3] - 1];
				v.UV = temp_uvs[uvIndex[3] - 1];

				mesh->vertBuff.push_back(v);								//last triangle added to vertBuffer
				indBuffer.push_back(mesh->vertBuff.size() - 1);
			}

			else
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return NULL;
			}	
		}
	}

	for (int i = 0; i < indBuffer.size(); i++)						//save all indices in a list with right type (int) 
	{
		mesh->IndBuff.push_back(int(indBuffer[i]));
	}
	//vertBuff = vertBuffer;
	mesh->setupVBO(&mesh->vertBuff[0], mesh->vertBuff.size());					//setup buffers with created mesh
	mesh->setupIBO(&indBuffer[0], indBuffer.size());
	return mesh;

}

std::vector<MeshResources::Vertex> MeshResources::getVertBuffer()
{
	return vertBuff;
}

std::vector<int> MeshResources::getIndBuffer()
{
	return IndBuff;
}

MeshResources* MeshResources::loadOBJfile(const char* filepath)
{
	MeshResources* mesh = new MeshResources();
	std::vector<unsigned int> vIndices, uvIndices, normalIndices;
	std::vector<Vector3D> temp_vertices;
	std::vector<Vector3D> temp_uvs;
	std::vector<Vector3D> temp_normals;

	std::vector<Vertex> vertBuffer;
	std::vector<GLuint> indBuffer;

	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		printf("Cannot open file! \n");
		return NULL;
	}

	//abbreviatons in a OBJ file
	//v is a vertex
	//vt is the texture coordinate of one vertex
	//vn is the normal of one vertex
	//f is a face
	while (1)
	{

		char lineHeader[128];							
		int res = fscanf(file, "%s", lineHeader);		
		if (res == EOF)									
		{
			break;
		}

		if (strcmp(lineHeader, "v") == 0)											
		{
			Vector3D vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);			
			temp_vertices.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0)										
		{
			Vector3D uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			temp_uvs.push_back(uv);
		}

		else if (strcmp(lineHeader, "vn") == 0)											
		{
			Vector3D Normal;
			fscanf(file, "%f %f %f\n", &Normal[0], &Normal[1], &Normal[2]);
			temp_normals.push_back(Normal);
		}

		else if (strcmp(lineHeader, "f") == 0)										
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			int readFace = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (readFace == 9)
			{
				Vertex v;												
				v.Pos = temp_vertices[vertexIndex[0] - 1];				
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);
			}

			else if (readFace == 12)										
			{
				Vertex v;
				v.Pos = temp_vertices[vertexIndex[0] - 1];				
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);				

				v.Pos = temp_vertices[vertexIndex[0] - 1];				
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[3] - 1];				
				v.Norm = temp_normals[normalIndex[3] - 1];
				v.UV = temp_uvs[uvIndex[3] - 1];

				vertBuffer.push_back(v);								
				indBuffer.push_back(vertBuffer.size() - 1);
			}

			else
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return NULL;
			}
		}
	}
	//vertBuff = vertBuffer;
	mesh->setupVBO(&vertBuffer[0], vertBuffer.size());					
	mesh->setupIBO(&indBuffer[0], indBuffer.size());
	return mesh;
}

