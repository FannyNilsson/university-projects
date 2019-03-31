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

void GraphicsNode::setHE_mesh(shared_ptr<Half_Edge_Mesh> heMesh)
{
	He_mesh = heMesh;								
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

shared_ptr<Half_Edge_Mesh> GraphicsNode::getHE_mesh()
{
	return He_mesh;
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
	TextRe->bindTexture();																//vi binder v�r texture
	Mesh->draw();																		//meshen renderas
}

void GraphicsNode::drawHE()
{
	ShaderOb->modifyUniformMatrix4("roteraMatrix", transform.GetMatrix());				//setup the shader via the transform matrix (same function as above but for Half edge meshes)
	ShaderOb->modifyUniformMatrix4("normalMat", cam.GetMatrix());
	TextRe->bindTexture();																
	He_mesh->DrawHE();																	
}