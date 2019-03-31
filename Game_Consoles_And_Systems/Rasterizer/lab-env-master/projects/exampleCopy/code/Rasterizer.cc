#include "config.h"
#include "Rasterizer.h"

Rasterizer::Rasterizer()
{

}

Rasterizer::~Rasterizer()
{

}

void Rasterizer::Init(int frameBufferWidth, int frameBufferHeight)
{
	this->frameBufferHeight = frameBufferHeight;
	this->frameBufferWidth = frameBufferWidth;
	Image_Buffer = new unsigned char[frameBufferWidth * frameBufferHeight * 3];				//size of the array is the amount of pixels * 3 (3 color values in 1 pixel)
	
	for (int i = 0; i < frameBufferHeight * frameBufferWidth * 3; i++)						//loop through image_buffer and clear it so the same picture isn't saved at each frame
	{
		Image_Buffer[i] = 255;															    
	}
	zbuffer = std::vector<float>(frameBufferHeight * frameBufferWidth, 1);					
	ViewProjMat();
}


void Rasterizer::ViewProjMat()											//setup for view och proj matrices
{
	Matrix3D matProj;							
	float fov = 90;
	float S = 1 / (tan((fov / 2) * (M_PI / 180)));
	float f = 10.0f;													//farclip plane (everything behind should NOT be rendered)
	float n = 1.0f;														//nearclip plane (everything in front of this should NOT be rendered)
	matProj[0][0] = S;
	matProj[1][1] = S;
	matProj[2][2] = -(f / (f - n));
	matProj[2][3] = -((f*n) / (f - n));
	matProj[3][2] = -1;
	matProj[3][3] = 0;

	Vector3D eye(0, 0, 5);												//where camera is placed
	Vector3D target(0, 0, 10);											//what camera is looking at
	Vector3D up(0,1,0);													//"explains" to the camera what is up and what is down 
	Vector3D zaxis = (target - eye).normal();							// The "forward" vector.
	Vector3D xaxis = (up.cross_product(zaxis)).normal();				// The "right" vector.
	Vector3D yaxis = zaxis.cross_product(xaxis);						// The "up" vector.

															
	Matrix3D viewMatrix;												// create view-matrix
	viewMatrix[0][0] = xaxis[0];
	viewMatrix[0][1] = yaxis[0];
	viewMatrix[0][2] = zaxis[0];
	viewMatrix[0][3] = 0;

	viewMatrix[1][0] = xaxis[1];
	viewMatrix[1][1] = yaxis[1];
	viewMatrix[1][2] = zaxis[1];
	viewMatrix[1][3] = 0;

	viewMatrix[2][0] = xaxis[2];
	viewMatrix[2][1] = yaxis[2];
	viewMatrix[2][2] = zaxis[2];
	viewMatrix[2][3] = 0;

	viewMatrix[3][0] = -(eye.dotproduct(xaxis));
	viewMatrix[3][1] = -(eye.dotproduct(yaxis));
	viewMatrix[3][2] = -(eye.dotproduct(zaxis));
	viewMatrix[3][3] = 1;

	view = viewMatrix;													
	proj = matProj;
	

	//Vector3D vec(0, 0, -10);
	//vec = matProj * vec;
	//vec.divideByW();
	//std::cout << vec.getZ();

}

void Rasterizer::SetupAndDraw(std::vector<MeshResources::Vertex> vertex_list, std::vector<int> indexbuffer, Matrix3D modelmatrix, unsigned char* texture, int textWidth, int texHeight)
{
	lambda = [texture, textWidth, texHeight](Vector3D uv, Vector3D normal)->Color						//declare lambda function
	{
		Color col;																						
		if (uv.getY() >= 1)
		{
			uv.setY(0.99f);
		}

		if (uv.getX() >= 1)
		{
			uv.setX(0.99f);
		}

		int p = (int)(uv.getY() * texHeight) * textWidth * 3 + (int)(uv.getX() * textWidth) * 3;		//correct pixel is collected. Since the buffer is 1D it needs to be multiplied with height and width to get the correct row - then add x*width to get to the correct column. Mult w 3 because every pixel has three values (RGB)

		col.red = texture[p];																			
		col.green = texture[p + 1];																		
		col.blue = texture[p + 2];

		return col;
	};

	MVP = proj * view * modelmatrix;														

	std::vector<MeshResources::Vertex> temp_vert;
	MeshResources::Vertex temp;
	for (int i = 0; i < vertex_list.size(); i++)
	{
		temp.Pos = MVP * vertex_list[i].Pos;												//all vertices multiplied w MVP matrix to put them in right space (NDC space!)
		temp.Pos.divideByW();																
		temp.Pos[0] = temp.Pos[0] * frameBufferWidth;										//scale				
		temp.Pos[1] = temp.Pos[1] * frameBufferHeight;
		
		temp.Norm = vertex_list[i].Norm;													
		temp.UV = vertex_list[i].UV;
		temp_vert.push_back(temp);
	}

	for (int j = 0; j < indexbuffer.size(); j += 3)											
	{
		RasterizeTriangle(temp_vert[indexbuffer[j]], temp_vert[indexbuffer[j + 1]], temp_vert[indexbuffer[j + 2]]);
	}
}

bool Rasterizer::Interpolate(OrganizedTriangle triangle, MeshResources::Vertex* pointInTriangle)
{
	float TotalArea;																					
	float area[3];																						
	TotalArea = getArea(triangle.LeftVertex.Pos, triangle.MidVertex.Pos, triangle.RightVertex.Pos);
	area[0] = getArea(triangle.LeftVertex.Pos, triangle.MidVertex.Pos, pointInTriangle->Pos);				
	area[1] = getArea(triangle.LeftVertex.Pos, triangle.RightVertex.Pos, pointInTriangle->Pos);			
	area[2] = getArea(triangle.RightVertex.Pos, triangle.MidVertex.Pos, pointInTriangle->Pos);			

	if (TotalArea == 0.0f)
		return false;
	float percentage[3];																				
	percentage[0] = area[0] / TotalArea;																//divide w total area to get precentage of each color to be used
	percentage[1] = area[1] / TotalArea;
	percentage[2] = area[2] / TotalArea;

	float interpolatedTextureCoordinates[] = { 0, 0 };													// save interpolated UV-values
	interpolatedTextureCoordinates[0] += percentage[0] * triangle.RightVertex.UV[0];					//multiply with correct vertex to get the value in right percentage (UV)
	interpolatedTextureCoordinates[0] += percentage[1] * triangle.MidVertex.UV[0];
	interpolatedTextureCoordinates[0] += percentage[2] * triangle.LeftVertex.UV[0];

	interpolatedTextureCoordinates[1] += percentage[0] * triangle.RightVertex.UV[1];
	interpolatedTextureCoordinates[1] += percentage[1] * triangle.MidVertex.UV[1];
	interpolatedTextureCoordinates[1] += percentage[2] * triangle.LeftVertex.UV[1];

	float interpolatedNormalCoordinates[] = {0, 0, 0};
	interpolatedNormalCoordinates[0] += percentage[0] * triangle.RightVertex.Norm[0];					//multiply with correct vertex to get the value in right percentage (normal)
	interpolatedNormalCoordinates[0] += percentage[1] * triangle.MidVertex.Norm[0];
	interpolatedNormalCoordinates[0] += percentage[2] * triangle.LeftVertex.Norm[0];

	interpolatedNormalCoordinates[1] += percentage[0] * triangle.RightVertex.Norm[1];
	interpolatedNormalCoordinates[1] += percentage[1] * triangle.MidVertex.Norm[1];
	interpolatedNormalCoordinates[1] += percentage[2] * triangle.LeftVertex.Norm[1];

	interpolatedNormalCoordinates[2] += percentage[0] * triangle.RightVertex.Norm[2];
	interpolatedNormalCoordinates[2] += percentage[1] * triangle.MidVertex.Norm[2];
	interpolatedNormalCoordinates[2] += percentage[2] * triangle.LeftVertex.Norm[2];

	float depth = 0;
	depth = depth + triangle.LeftVertex.Pos[2] * percentage[0];
	depth = depth + triangle.MidVertex.Pos[2] * percentage[1];
	depth = depth + triangle.RightVertex.Pos[2] * percentage[2];

	pointInTriangle->Pos = Vector3D(pointInTriangle->Pos[0], pointInTriangle->Pos[1], depth);							//update the point in the triangle with it's now interpolated values
	pointInTriangle->UV = Vector3D(interpolatedTextureCoordinates[0], interpolatedTextureCoordinates[1], 0);			//save new UV values
	pointInTriangle->Norm = Vector3D(interpolatedNormalCoordinates[0], interpolatedNormalCoordinates[1], interpolatedNormalCoordinates[2]);
	return true;
}

void Rasterizer::RasterizeTriangle(MeshResources::Vertex one, MeshResources::Vertex two, MeshResources::Vertex three)
{
	OrganizedTriangle triangle(one, two, three);																	//create a triangle of vertices one, two, three
	
	std::vector<int> AB = Bresenham(triangle.LeftVertex, triangle.MidVertex);
	std::vector<int> AC = Bresenham(triangle.LeftVertex, triangle.RightVertex);
	std::vector<int> BC = Bresenham(triangle.MidVertex, triangle.RightVertex);

	if (AC.size() == 0)
		return;
	
	int i = 0;
	int j = 0;
	int x;
	
	if (triangle.LeftVertex.Pos[0] != triangle.MidVertex.Pos[0])
	{
		x = AB[0];

		//First half of triangle
		while (x < AB[AB.size()-2])
		{
			while (i + 2 < AB.size() && x == AB[i + 2])
			{
				i += 2;
			}
			int top = AB[i + 1];

			while (j + 2 < AC.size() && x == AC[j + 2])
			{
				j += 2;
			}
			int bot = AC[j + 1];

			int temp;
			if (top < bot)
			{
				temp = top;
				top = bot;
				bot = temp;
			}

			for (; bot <= top; bot++)
			{
				if (bot > 0 && bot < frameBufferHeight && x > 0 && x < frameBufferWidth)
					ColorPixel(triangle, x, bot);
			}
			x++;
		}
	}
	
	
	if (triangle.MidVertex.Pos[0] != triangle.RightVertex.Pos[0])
	{
		x = BC[0];
		i = 0;
		//Second half of triangle
		while (x < BC[BC.size() - 2]) // BC[BC.size() - 2] == last 'x' of list
		{
			while (i + 2 < BC.size() && x == BC[i + 2])
			{
				i += 2;
			}
			int top = BC[i + 1];

			while (j + 2 < AC.size() && x == AC[j + 2])
			{
				j += 2;
			}
			int bot = AC[j + 1];

			int temp;
			if (top < bot)
			{
				temp = top;
				top = bot;
				bot = temp;
			}

			for (; bot <= top; bot++)
			{
				if (bot > 0 && bot < frameBufferHeight && x > 0 && x < frameBufferWidth)
					ColorPixel(triangle, x, bot);
			}
			x++;
		}
	}
}

std::vector<int> Rasterizer::Bresenham(MeshResources::Vertex vertex1, MeshResources::Vertex vertex2)
{
	
	std::vector<int> edge;
	X_Y A = X_Y(vertex1.Pos[0], vertex1.Pos[1]);
	X_Y B = X_Y(vertex2.Pos[0], vertex2.Pos[1]);

	int octant = SwitchFromOctantZeroTo(&A, &B);
	float dVar = 2 * (B.y - A.y) - (B.x - A.x);

	int x = (int)A.x;
	int y = (int)A.y;
	for (;x < B.x; x++)
	{
		if (dVar < 0)
			dVar = dVar + 2 * (B.y - A.y);
		else
		{
			y++;
			dVar = dVar + 2 * ((B.y - A.y) - (B.x - A.x));
		}

		X_Y edgePoint = X_Y(x, y);
		SwitchToOctantZeroFrom(octant, A, &edgePoint);
		edge.push_back((int)edgePoint.x);
		edge.push_back((int)edgePoint.y);
	}
	return edge;
}

float Rasterizer::getArea(Vector3D A, Vector3D B, Vector3D C)							//calculate area of a triangle
{
	float Ax, Bx, Cx, Ay, By, Cy, area;
	Ax = A.getX();
	Ay = A.getY();
	Bx = B.getX();
	By = B.getY();
	Cx = C.getX();
	Cy = C.getY();

	area = abs(Ax*(By - Cy) + Bx*(Cy - Ay) + Cx*(Ay - By));
	return area;
}

unsigned char* Rasterizer::GetImageBuffer()
{
	return Image_Buffer;
}

void Rasterizer::ClearImageBuffer()
{
	for (int i = 0; i < frameBufferHeight * frameBufferWidth * 3; i++)					//loop through image_buffer and clear it so not every frame is the same picture
	{
		Image_Buffer[i] = 255;															
	}

	for(int j = 0; j < zbuffer.size(); j++)												//clear z buffer as well
	{
		zbuffer[j] = 1.0f;
	}
}

float Rasterizer::GetFrameBufferWidth()
{
	return frameBufferWidth;
}

float Rasterizer::GetFrameBufferHeight()
{
	return frameBufferHeight;
}

void Rasterizer::ColorPixel(OrganizedTriangle triangle, int posX, int posY)
{
	MeshResources::Vertex Pixel;
	Pixel.Pos = Vector3D(posX, posY, 0);										//create vertex which represents the pixel to be colored
	if (!Interpolate(triangle, &Pixel))											//interpolate the triangle at the given pixel
		return;
	if (Pixel.Pos[2] > zbuffer[posY * frameBufferWidth + posX])					//if z value for the pixel is bigger than the z buffers value at this position, it means that it shouldn't be rendered everything behind that should not be rendered.
	{
		return;
	}

	zbuffer[posY * frameBufferWidth + posX] = Pixel.Pos[2];						//z buffer saves the pixel value since it is clear that it is in front.
	Color col = lambda(Pixel.UV, Pixel.Norm);									//collect color values for given pixel in the texture
	Image_Buffer[(posY * frameBufferWidth * 3) + (posX * 3)] = col.red;			//set values
	Image_Buffer[(posY * frameBufferWidth * 3) + (posX * 3) + 1] = col.green;	
	Image_Buffer[(posY * frameBufferWidth * 3) + (posX * 3) + 2] = col.blue;	
}

Matrix3D Rasterizer::getMVP()
{
	return MVP;
}

void Rasterizer::SwitchToOctantZeroFrom(int octant, X_Y A, X_Y* B)				//move back to correct octant after bresenham is done
{
	float temp;																	

	switch (octant)
	{
	case 0:																		//if in octant 1 nothing needs to be done since bresenham can handle that(angle within 45 grader)
		break;

	case 1:
		B->x -= A.x;										//assume that One is in origo
		B->y -= A.y;
		temp = B->x;										//save x value of B before switching
		B->x = B->y;
		B->y = temp;										//switch
		B->x += A.x;										
		B->y += A.y;
		break;
	
	case 6:													//depending on in which octant we're in, B needs to be moved in different ways.
		B->x -= A.x;
		B->y -= A.y;
		temp = B->y;
		B->y = -(B->x);
		B->x = temp;
		B->x += A.x;
		B->y += A.y;
		break;

	case 7:
		B->x -= A.x;
		B->y -= A.y;
		B->y = -(B->y);
		B->x += A.x;
		B->y += A.y;
		break;
	}
}

int Rasterizer::SwitchFromOctantZeroTo(X_Y* A, X_Y* B)							//called before bresenham since lines need to be within 45 degrees for bresenham to work
{
	if (B->x >= A->x)															//check if A is to the left of not. If A allways is to the left one can assume from origo and only 4 cases are needed.
		if (B->y > A->y)
			if ((B->y - A->y) < (B->x - A->x))
				return 0;
			else
			{
				B->x -= A->x;
				B->y -= A->y;
				float temp = B->x;
				B->x = B->y;
				B->y = temp;
				B->x += A->x;
				B->y += A->y;
				return 1;
			}
		else
			if ((A->y - B->y) < (B->x - A->x))
			{
				B->x -= A->x;
				B->y -= A->y;
				B->y = -(B->y);
				B->x += A->x;
				B->y += A->y;
				return 7;
			}
			else
			{
				B->x -= A->x;
				B->y -= A->y;
				float temp = B->x;
				B->x = -(B->y);
				B->y = temp;
				B->x += A->x;
				B->y += A->y;
				return 6;
			}
}

