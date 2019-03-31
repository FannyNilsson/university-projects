#pragma once
#include "core/app.h"
#include "render/window.h"
#include <iostream>
#include "Matrix3D.h"
#include <memory>
#include "ShaderObject.h"


class LightNode
{
private:
	shared_ptr<ShaderObject> ShaderOb;
	Vector3D DiffColor;															// Color of that point light
	Vector3D SpecColor;
	Vector3D LightPos;
	float intensity;

public:
	void setSpecColor(float r, float g, float b);
	void setSpecColor(Vector3D col);
	void setDiffColor(float r, float g, float b);
	void setDiffColor(Vector3D col);
	void setLightPos(Vector3D posi);
	void setLightPos(float x, float y, float z);
	void setIntensity(float intense);
	void setShader(shared_ptr<ShaderObject> shade);								//denna ändrar bara light delarna i shadern
	void generateLight();
	~LightNode();
	LightNode();
};