#include "config.h"
#define STB_IMAGE_IMPLEMENTATION
#include "TextureResource.h"




TextureResource::TextureResource():Texture_obj(0)
{
}


TextureResource::~TextureResource()
{
	glDeleteBuffers(1, &Texture_obj);
}

void TextureResource::load_file(char* filename)
{
	int x, y, n;
	unsigned char *image = stbi_load(filename, &x, &y, &n, 4);
	if (image == nullptr)
	{
		std::cout << "Image could not load." << std::endl;
		return;
	}
	
	glGenTextures(1, &Texture_obj);														//create a texture
	glBindTexture(GL_TEXTURE_2D, Texture_obj);											//bind (activate) texture 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		//"explain to the shader" how it should handle the picture if it is zoomed in. (Use mip-mapping, not just random pixels)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					//"explain to the shader" how it should handle the picture if it is zoomed out.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);	//load data for picture to the texture

	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateTextureMipmap(Texture_obj);												

	glBindTexture(GL_TEXTURE_2D, 0);													//"deactivate" texture

	stbi_image_free(image);
}

void TextureResource::bindTexture()														//bind (activates) texturen. Called upon when texture is going to be used 
{
	glBindTexture(GL_TEXTURE_2D, Texture_obj);
}

unsigned char* TextureResource::BuildText(const char* filename, int width, int height)
{
	int n;
	unsigned char *image = stbi_load(filename, &width, &height, &n, 3);
	//unsigned char *image = new unsigned char[width * height * 3];
	
	return image;
}

void TextureResource::Load_file_rast(unsigned char* image_buffer, int width, int height)
{
	if (Texture_obj == 0)
	{
		glGenTextures(1, &Texture_obj);														//create a texture
	}

	glBindTexture(GL_TEXTURE_2D, Texture_obj);											//bind (activate) texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		//explains to the shader how to handle the picture if it is zoomed in (use mip-maping, not just random pixels)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					//explains to the shader how to handle the picture if it is zoomed out

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_buffer);	//load the data for the picture to the texture

	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateTextureMipmap(Texture_obj);												

	glBindTexture(GL_TEXTURE_2D, 0);													//"deactivate" texture
}


