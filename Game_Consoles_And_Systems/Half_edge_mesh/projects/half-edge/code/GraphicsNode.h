#pragma once

#include "core/app.h"
#include "render/window.h"
#include "MeshResources.h"
#include "ShaderObject.h"
#include "TextureResource.h"
#include "Matrix3D.h"
#include <memory>
#include <iostream>
#include "Half_Edge_Mesh.h"

using namespace std;

class GraphicsNode
{
private:
	shared_ptr<MeshResources> Mesh;
	shared_ptr<TextureResource> TextRe;
	shared_ptr<ShaderObject> ShaderOb;
	shared_ptr<Half_Edge_Mesh> He_mesh;
	Matrix3D transform;
	Matrix3D cam;
	Vector3D vec;

public:
	void setShader(shared_ptr<ShaderObject> shade);
	void setMesh(shared_ptr<MeshResources> meshRe);
	void setText(shared_ptr<TextureResource> tex);
	void setHE_mesh(shared_ptr<Half_Edge_Mesh> heMesh);


	void setTransform(const Matrix3D& trans);
	void setCam(const Matrix3D& camera);
	void setVec(const Vector3D& vec);
	shared_ptr<ShaderObject> getShader();
	shared_ptr<MeshResources> getMesh();
	shared_ptr<TextureResource> getTexture();
	shared_ptr<Half_Edge_Mesh> getHE_mesh();
	void draw();
	void drawHE();

	GraphicsNode();
	~GraphicsNode();
};