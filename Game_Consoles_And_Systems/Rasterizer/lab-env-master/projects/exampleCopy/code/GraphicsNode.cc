#include "config.h"
#include "GraphicsNode.h"




GraphicsNode::GraphicsNode()
{
}


GraphicsNode::~GraphicsNode()
{

}

void GraphicsNode::setShader(shared_ptr<ShaderObject> shade)
{
	ShaderOb = shade;																					
}

void GraphicsNode::setMesh(shared_ptr<MeshResources> meshRe)
{
	Mesh = meshRe;								
}

void GraphicsNode::setText(shared_ptr<TextureResource> tex)
{
	TextRe = tex;							
}

shared_ptr<ShaderObject> GraphicsNode::getShader()
{
	return ShaderOb;
}

shared_ptr<MeshResources> GraphicsNode::getMesh()
{
	return Mesh;
}

shared_ptr<TextureResource> GraphicsNode::getTexture()
{
	return TextRe;
}

void GraphicsNode::setTransform(const Matrix3D& trans)
{
	transform = trans;																	
}

void GraphicsNode::setCam(const Matrix3D& camera)
{
	cam = camera;
}

void GraphicsNode::setVec(const Vector3D& vect)
{
	vec = vect;
}

void GraphicsNode::draw()
{
	ShaderOb->modifyUniformMatrix4("roteraMatrix", transform.GetMatrix());				//setup the shader via the transform matrix
	ShaderOb->modifyUniformMatrix4("normalMat", cam.GetMatrix());
	TextRe->bindTexture();																//bind the texture
	Mesh->draw();																		//render mesh
}
