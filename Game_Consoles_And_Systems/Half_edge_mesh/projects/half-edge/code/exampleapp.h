#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "MeshResources.h"
#include "TextureResource.h"
#include "ShaderObject.h"
#include "Matrix3D.h"
#include <memory>
#include "GraphicsNode.h"
#include "LightNode.h"
#include "Rasterizer.h"
#include "Half_Edge_Mesh.h"

namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();
	
private:
	Matrix3D camMat;
	Matrix3D move_;
	Matrix3D temp;
	float intensity = 0.0f;
	float angle = 0.0f;
	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
	LightNode light;
	std::shared_ptr<MeshResources> mesh;
	std::shared_ptr<TextureResource> text;
	std::shared_ptr<ShaderObject> soso;

	std::shared_ptr<TextureResource> text2;
	std::shared_ptr<MeshResources> mesh2;
	std::shared_ptr<TextureResource> text3;
	std::shared_ptr<MeshResources> mesh3;
	GraphicsNode node;
	GraphicsNode node2;
	GraphicsNode node3;
	GraphicsNode HE_mesh_node;

	GraphicsNode Graprast;
	MeshResources quad;
	std::shared_ptr<MeshResources> mesh1;
	std::shared_ptr<TextureResource> RastText;
	Matrix3D temp4;
	
	Rasterizer Raster;
	std::vector<MeshResources::Vertex> testTriangle;
	std::vector<int> testIndex;
	MeshResources::Vertex v1, v2, v3;
	unsigned char* texture_buffer;

	std::shared_ptr<Half_Edge_Mesh> he_mesh;
};
} // namespace Example
