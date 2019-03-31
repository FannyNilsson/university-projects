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
		//triangel sedd som "�vre till v�nster delen av rektangeln"
		0, 2, 1,

		//triangel sedd som "undre till h�ger delen av rektangeln"
		0, 3, 2,
	};
	setupIBO(Indices,6);
}

void MeshResources::setupVBO(Vertex* buffer, int numOfVertices)
{
	glGenBuffers(1, &Vertex_buffer);																//buffern skapas
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_buffer);													//buffern aktiveras
	glBufferData(GL_ARRAY_BUFFER, numOfVertices*sizeof(Vertex), buffer, GL_STATIC_DRAW);			//skapar och initialiserar buffern. H�r ger man buffern en viss storlek i bytes(storleken av arrayn buffer). I vertex_buffer "matas" arrayn buffer in
	glBindBuffer(GL_ARRAY_BUFFER, 0);																//deaktiverar buffern
	return;
}

void MeshResources::setupIBO(GLuint * indices, int count)											//funktionen tar argumentet som �r en index buffer (eg en pekare till en buffer)
{
	glGenBuffers(1, &Index_buffer);														//skapar buffern
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index_buffer);								//aktiverar buffern
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(int), indices, GL_STATIC_DRAW);		//data "laddas in" om den
	this->indexCount = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
bool MeshResources::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, Vertex_buffer);															//aktiverar buffern
	glEnableVertexAttribArray(0);																			//enablar v�ra vertexattributes hos shadern
	glEnableVertexAttribArray(1);																			//enablar v�ra vertexattributes hos shadern
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);								//definerar en array med attribut data. 3 betyder hur m�nga element som ska anv�ndas. Storleken av v�r struct vertex ger v�r offset NULL �r inneb�r att vi b�rjar fr�n b�rjan.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D)));		//samma som ovanst�ende men denna g�ng b�rjar vi efter 3:e elementet f�r att forts�tta d�r vi slutatde
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D) * 2));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index_buffer);													//aktiverar min index buffer
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

	//f�rkortningar i en OBJ fil och vad de eg �r
	//v is a vertex
	//vt is the texture coordinate of one vertex
	//vn is the normal of one vertex
	//f is a face
	while (1)
	{

		char lineHeader[128];							//vi antar bara att f�rsta ordet p� varje rad inte kommer vara l�ngre �n 128 karakt�rer
		int res = fscanf(file, "%s", lineHeader);		// f�rsta ordet p� raden l�ses mha funktionen fscanf.
		if (res == EOF)									//om vi �r vid End Of File (EOF) vill vi sluta l�sa och breaka loopen
		{
			break;
		}

		if (strcmp(lineHeader, "v") == 0)											//om f�rsta "ordet" �r ett "v" inneb�r det att resten av raden handlar om vertices
		{
			Vector3D vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);			//vi lagrar alla vertices i en Vector3D som sedan sparas i temp_vertices
			temp_vertices.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0)										//h�r g�r vi likadant men med uv-koordinaterna ist
		{
			Vector3D uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			temp_uvs.push_back(uv);
		}

		else if (strcmp(lineHeader, "vn") == 0)										//�ven h�r h�nder samma men med normalen ist.	
		{
			Vector3D Normal;
			fscanf(file, "%f %f %f\n", &Normal[0], &Normal[1], &Normal[2]);
			temp_normals.push_back(Normal);
		}

		else if (strcmp(lineHeader, "f") == 0)										//h�r g�r vi likadant men med faces vilket g�r att koden blir l�ngre och ser mer komplicerad ut
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			
			int readFace = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (readFace == 9)
			{
				Vertex v;												//Skapar en struct som ska spara alla vertiser och dess pos, uv och normaler
				v.Pos = temp_vertices[vertexIndex[0] - 1];				//h�r sparas vertis 1. (temp_vertices �r alla sparade vertiser, medan VertexIndex visar vilken vertis i facet vi vill komma �t) 
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertis 2 sparas h�r med alla dess v�rden
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertis 3 sparas med alla dess v�rden
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);
			}

			else if(readFace == 12)										//om vi OBJ filen inneh�ller  quads vill vi kunna dela upp dessa f�r att kunna ladda in filen
			{
				Vertex v;
				v.Pos = temp_vertices[vertexIndex[0] - 1];				//vertix 0
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertis 1
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertis 2
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);				//nu har hela f�rsta triangeln lagts till i vertBuffer (0,1,2)

				v.Pos = temp_vertices[vertexIndex[0] - 1];				//samma sak m�ste g�ras igen men med den andra triangeln
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertis 2
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				mesh->vertBuff.push_back(v);
				indBuffer.push_back(mesh->vertBuff.size() - 1);

				v.Pos = temp_vertices[vertexIndex[3] - 1];				//vertis 4
				v.Norm = temp_normals[normalIndex[3] - 1];
				v.UV = temp_uvs[uvIndex[3] - 1];

				mesh->vertBuff.push_back(v);								//sista triangeln �r nu inladdad i vertBuffer
				indBuffer.push_back(mesh->vertBuff.size() - 1);
			}

			else
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return NULL;
			}	
		}
	}

	for (int i = 0; i < indBuffer.size(); i++)						//Sparar alla index i en lista med r�tt typ (ints)
	{
		mesh->IndBuff.push_back(int(indBuffer[i]));
	}
	//vertBuff = vertBuffer;
	mesh->setupVBO(&mesh->vertBuff[0], mesh->vertBuff.size());					//vi s�tter upp v�ra buffers med meshen vi skapade. Buffersen h�ller infon som filen vi laddade in gav.
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

	//f�rkortningar i en OBJ fil och vad de eg �r
	//v is a vertex
	//vt is the texture coordinate of one vertex
	//vn is the normal of one vertex
	//f is a face
	while (1)
	{

		char lineHeader[128];							//vi antar bara att f�rsta ordet p� varje rad inte kommer vara l�ngre �n 128 karakt�rer
		int res = fscanf(file, "%s", lineHeader);		// f�rsta ordet p� raden l�ses mha funktionen fscanf.
		if (res == EOF)									//om vi �r vid End Of File (EOF) vill vi sluta l�sa och breaka loopen
		{
			break;
		}

		if (strcmp(lineHeader, "v") == 0)											//om f�rsta "ordet" �r ett "v" inneb�r det att resten av raden handlar om vertices
		{
			Vector3D vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);			//vi lagrar alla vertices i en Vector3D som sedan sparas i temp_vertices
			temp_vertices.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0)										//h�r g�r vi likadant men med uv-koordinaterna ist
		{
			Vector3D uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			temp_uvs.push_back(uv);
		}

		else if (strcmp(lineHeader, "vn") == 0)										//�ven h�r h�nder samma men med normalen ist.	
		{
			Vector3D Normal;
			fscanf(file, "%f %f %f\n", &Normal[0], &Normal[1], &Normal[2]);
			temp_normals.push_back(Normal);
		}

		else if (strcmp(lineHeader, "f") == 0)										//h�r g�r vi likadant men med faces vilket g�r att koden blir l�ngre och ser mer komplicerad ut
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			int readFace = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (readFace == 9)
			{
				Vertex v;												//Skapar en struct som ska spara alla vertiser och dess pos, uv och normaler
				v.Pos = temp_vertices[vertexIndex[0] - 1];				//h�r sparas vertis 1. (temp_vertices �r alla sparade vertiser, medan VertexIndex visar vilken vertis i facet vi vill komma �t) 
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertis 2 sparas h�r med alla dess v�rden
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertis 3 sparas med alla dess v�rden
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);
			}

			else if (readFace == 12)										//om vi OBJ filen inneh�ller  quads vill vi kunna dela upp dessa f�r att kunna ladda in filen
			{
				Vertex v;
				v.Pos = temp_vertices[vertexIndex[0] - 1];				//vertix 0
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[1] - 1];				//vertis 1
				v.Norm = temp_normals[normalIndex[1] - 1];
				v.UV = temp_uvs[uvIndex[1] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertis 2
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);				//nu har hela f�rsta triangeln lagts till i vertBuffer (0,1,2)

				v.Pos = temp_vertices[vertexIndex[0] - 1];				//samma sak m�ste g�ras igen men med den andra triangeln
				v.Norm = temp_normals[normalIndex[0] - 1];
				v.UV = temp_uvs[uvIndex[0] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[2] - 1];				//vertis 2
				v.Norm = temp_normals[normalIndex[2] - 1];
				v.UV = temp_uvs[uvIndex[2] - 1];

				vertBuffer.push_back(v);
				indBuffer.push_back(vertBuffer.size() - 1);

				v.Pos = temp_vertices[vertexIndex[3] - 1];				//vertis 4
				v.Norm = temp_normals[normalIndex[3] - 1];
				v.UV = temp_uvs[uvIndex[3] - 1];

				vertBuffer.push_back(v);								//sista triangeln �r nu inladdad i vertBuffer
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
	mesh->setupVBO(&vertBuffer[0], vertBuffer.size());					//vi s�tter upp v�ra buffers med meshen vi skapade. Buffersen h�ller infon som filen vi laddade in gav.
	mesh->setupIBO(&indBuffer[0], indBuffer.size());
	return mesh;
}

