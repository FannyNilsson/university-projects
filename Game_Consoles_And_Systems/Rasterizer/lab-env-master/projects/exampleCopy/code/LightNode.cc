#include "config.h"
#include "LightNode.h"




LightNode::LightNode()
{
}


LightNode::~LightNode()
{

}

void LightNode::setShader(shared_ptr<ShaderObject> shade)
{
	ShaderOb = shade;														
}

void LightNode::setDiffColor(float r, float g, float b)						
{
	Vector3D col(r, g, b);
	DiffColor = col;
}

void LightNode::setDiffColor(Vector3D col)									
{
	DiffColor = col;
}

void LightNode::setSpecColor(float r, float g, float b)						
{
	Vector3D col(r, g, b);
	SpecColor = col;
}

void LightNode::setSpecColor(Vector3D col)									
{
	SpecColor = col;
}

void LightNode::setLightPos(float x, float y, float z)						
{
	Vector3D pos(x, y, z);
	LightPos = pos;
}

void LightNode::setLightPos(Vector3D pos)									
{
	LightPos = pos;
}

void LightNode::setIntensity(float intense)									
{
	intensity = intense;
	DiffColor = DiffColor* intensity;
	SpecColor = SpecColor* intensity;
}

void LightNode::generateLight()													//apply all data to the shader
{
	ShaderOb->modifyUniformVector4("diffuseColor", DiffColor.getVec());			//using the ShaderOb (smart pointer) to "get to the shader"
	ShaderOb->modifyUniformVector4("specColor", SpecColor.getVec());
	ShaderOb->modifyUniformVector4("lightPos", LightPos.getVec());
}