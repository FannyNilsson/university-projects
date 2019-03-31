#pragma once
#include <functional>
#include "core/app.h"
#include "MeshResources.h"
#include "render/window.h"
#include <iostream>


class Rasterizer
{
public:
	struct Color																						//en struct som f�rvarar f�rg v�rden
	{
		int red;
		int green;
		int blue;

		//Color()
		//{
		//	red = 0.0f;
		//	green = 0.0f;
		//	blue = 1.0f;
		//}
	};

	struct X_Y
	{
		float x;
		float y;
		
		X_Y(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
	};

	struct OrganizedTriangle																			//en struct som ordnar triangelns vertiser i r�tt ordning f�r att bresenham ska fungera korrekt sedan
	{
		MeshResources::Vertex MidVertex;
		MeshResources::Vertex LeftVertex;
		MeshResources::Vertex RightVertex;

		OrganizedTriangle(MeshResources::Vertex A, MeshResources::Vertex B, MeshResources::Vertex C)	//konstruktorn kollar ifall ordningen p� vertiserna st�mmer mha dess x-v�rden
		{
			LeftVertex = A;
			MidVertex = B;
			RightVertex = C;

			if (MidVertex.Pos[0] < LeftVertex.Pos[0])													//ex om vertisen midvertisen ligger mer till v�nster �n den som ska b�rja (left) d�r m�ste vi flippa efetrsom bresenham b�rjar d�r i mitt fall
			{
				SwitchVertices(&MidVertex, &LeftVertex);
			}

			if (LeftVertex.Pos[0] > RightVertex.Pos[0])
			{
				SwitchVertices(&LeftVertex, &RightVertex);
			}

			if (MidVertex.Pos[0] > RightVertex.Pos[0])
			{
				SwitchVertices(&MidVertex, &RightVertex);
			}

		}

		void SwitchVertices(MeshResources::Vertex* A, MeshResources::Vertex* B)							//vi flyttar plats p� vart vertiserna �r f�r att f� r�tt ordning
		{
			MeshResources::Vertex temp = (*A);															//f�rst sparar vi v�rdet f�r A 
			(*A) = (*B);																				//A f�r sedan v�rdet som B f�rvarar
			(*B) = temp;																				//B f�r sedan v�rdet som A f�rvarade som nu f�rvaras i temp.
		}
	};

	std::function<Color(Vector3D uv, Vector3D normal)> lambda;
	void RasterizeTriangle(MeshResources::Vertex one, MeshResources::Vertex two, MeshResources::Vertex three);
	void ViewProjMat();
	bool Interpolate(OrganizedTriangle triangle, MeshResources::Vertex* pointInTriangle);
	void Init(int frameBufferWidth, int frameBufferHeight);
	void SetupAndDraw(std::vector<MeshResources::Vertex> vertex, std::vector<int> indexbuffer, Matrix3D modelmatrix, unsigned char* texture, int textWidth, int texHeight);
	std::vector<int> Bresenham(MeshResources::Vertex vertex1, MeshResources::Vertex vertex2);
	float getArea(Vector3D A, Vector3D B, Vector3D C);
	unsigned char* GetImageBuffer();
	float GetFrameBufferWidth();
	float GetFrameBufferHeight();
	Matrix3D getMVP();
	void ClearImageBuffer();
	void SwitchToOctantZeroFrom(int octant, X_Y One, X_Y* Two);
	int SwitchFromOctantZeroTo(X_Y* A, X_Y* B);
	void ColorPixel(OrganizedTriangle triangle, int posX, int posY);
	//void setCamera();
	Rasterizer();
	~Rasterizer();

	

private:
	std::vector<float> zbuffer;
	std::vector<float> indexbuffer;
	int frameBufferWidth;
	int frameBufferHeight;
	unsigned char* Image_Buffer;
	Matrix3D MVP;
	Matrix3D view;
	Matrix3D proj;

	GLuint glTex;
	void UpdateTexture();
};