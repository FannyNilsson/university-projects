//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{

	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
		float speed = 0.01f;

		switch (key)
		{
		//case GLFW_KEY_SPACE
		//	this->window->Close();
		case  GLFW_KEY_W:										//Apply speed in y-axis
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				this->move_.set(1, 3, speed);
			}
			else
			{
				this->move_.set(1, 3, 0);
			}
			break;

		case  GLFW_KEY_A:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				this->move_.set(0, 3, speed* -1.0f);
			}
			else
			{
				this->move_.set(0, 3, 0);
			}
			break;
		case  GLFW_KEY_S:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				this->move_.set(1, 3, speed* -1.0f);				//Apply -speed in y-axis
			}
			else
			{
				this->move_.set(1, 3, 0);
			}

			break;
		case  GLFW_KEY_D:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				this->move_.set(0, 3, speed);
			}
			else
			{
				this->move_.set(0, 3, 0);
			}
			break;
		case  GLFW_KEY_P:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				intensity = intensity + 0.5f;							//add intensity of the light by pressing P
			}
			else
			{
				intensity = intensity;
			}
			break;
		case  GLFW_KEY_O:
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				intensity = intensity - 0.5f;							//lower the intensity of the light by pressing O
			}
			else
			{
				intensity = intensity;
			}
			break;
		}



	});

	window->SetMousePressFunction([this](int32, int32 pressed, int32) 	//apply rotation of object
	{
		if (pressed == 1)												
		{
			angle = 1.0f;
		}
		else
		{
			angle = 0.0f;
		}

	});

	if (this->window->Open())
	{
		//glEnable(GL_CULL_FACE);
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		text = make_shared<TextureResource>();													//create smart pointer to an empty texture object
		text->load_file("resources/texture_pic.jpg");											//load the file of the texture

		text2 = make_shared<TextureResource>();
		text2->load_file("resources/bfs.jpg");

		text3 = make_shared<TextureResource>();
		text3->load_file("resources/unicorn.jpg");

		soso = make_shared<ShaderObject>();														//create smart pointer to an empty shaderobject
		soso->startProgram("vertex_shader.txt", "fragment_shader.txt");
		
		mesh = shared_ptr<MeshResources>(MeshResources::loadOBJfile("quad.obj"));				//set the smart pointer to point at mesh object 	
		mesh2 = shared_ptr<MeshResources>(MeshResources::loadOBJfile_Rast("cube.obj"));
		mesh3 = shared_ptr<MeshResources>(MeshResources::loadOBJfile("Gringotts_Dragon.obj"));
		node.setMesh(mesh);																		//set member variables
		node.setText(text);
		node.setShader(soso);

		he_mesh = shared_ptr<Half_Edge_Mesh>(Half_Edge_Mesh::loadOBJfile("cube.obj"));			//create smart pointer for half-edge mesh 
		HE_mesh_node.setHE_mesh(he_mesh);
		HE_mesh_node.setText(text);
		HE_mesh_node.setShader(soso);

		node2.setMesh(mesh2);																	//set member variables
		node2.setText(text2);
		node2.setShader(soso);

		node3.setMesh(mesh3);																	//set member variables
		node3.setText(text3);
		node3.setShader(soso);

		light.setShader(soso);																	//apply all values for light node. Start with shader
		light.setDiffColor(0.1, 0.1, 0.1);														//set diffusecolor
		light.setSpecColor(1.0, 1.0, 1.0);														//set specularcolor
		light.setLightPos(0.0f, 0.0f, 20.0f);													//set position for light
		light.setIntensity(2.5f);
		light.generateLight();


		RastText = make_shared<TextureResource>();
		
		quad.CreateQuad();
		//mesh1 = shared_ptr<MeshResources>(&quad);

		Graprast.setMesh(mesh);//shared_ptr<MeshResources>(&quad));
		Graprast.setText(text2);
		Graprast.setShader(soso);

		v1.Pos[0] = 0.0f;
		v1.Pos[1] = 0.0f;
		v1.UV[0] = 0.0f;
		v1.UV[1] = 0.0f;
		testTriangle.push_back(v1);

		v2.Pos[0] = 0.0f;
		v2.Pos[1] = 0.8f;
		v2.UV[0] = 0.0f;
		v2.UV[1] = 0.8f;
		testTriangle.push_back(v2);

		v3.Pos[0] = 1.0f;
		v3.Pos[1] = 1.0f;
		v3.UV[0] = 0.99f;
		v3.UV[1] = 0.99f;
		testTriangle.push_back(v3);

		
		testIndex.push_back(0);
		testIndex.push_back(1);
		testIndex.push_back(2);
		
		int width, height;
		Raster.Init(512, 512);
		texture_buffer = TextureResource::BuildText("resources/texture_pic.jpg",512,512);					//save image in texture_buffer

		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{

	Vector3D VEC_POS(0, 0, 10.0f);
	camMat.set_position(VEC_POS);																			//set camera position
	temp = temp *Matrix3D::scale(0.1f, 0.1f, 0.1f);															//scale down the object to appropriate size
	temp4 = temp*Matrix3D::scale(5.0f, 5.0f, 5.0f);
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();


		float vinkel = 1.0f;
		temp = move_*temp*Matrix3D::rotate_x(angle)*Matrix3D::rotate_y(angle);								
		//temp4 = move_*temp4*Matrix3D::rotate_x(angle)*Matrix3D::rotate_y(angle);
		node.setTransform(temp);
		node.setCam(camMat * temp);
		//node.draw();																						//render the mesh
		
		Matrix3D temp2;
		Matrix3D temp3;
		temp3 = temp;
		temp3 = temp*Matrix3D::scale(3.0f, 3.0f, 3.0f);
		temp2 = temp*Matrix3D::scale(0.9f, 0.9f, 0.9f);
		//temp4 = temp*Matrix3D::scale(3.0f, 3.0f, 3.0f);
		temp2.set_position(temp2.get_position() + Vector3D(0.5f, 0, 0));
		temp3.set_position(temp3.get_position() + Vector3D(-0.5f, 0, 0));
		//temp4.set_position(temp4.get_position() + Vector3D(-0.5f, -0.5f, 0));
		
		node2.setTransform(temp2);
		node2.setCam(camMat * temp);
		//node2.draw();

		node3.setTransform(temp3);
		node3.setCam(camMat * temp);
		//node3.draw();

		HE_mesh_node.setTransform(temp3);
		HE_mesh_node.setCam(camMat * temp);
		HE_mesh_node.drawHE();

		
		this->window->SwapBuffers();
	}
}

} // namespace Example