#pragma once
#include "core/app.h"
#include "render/window.h"
#include <iostream>

#include "stb_image.h"


class TextureResource
{
private:
	GLuint Texture_obj;
	unsigned char *image_;

public:
	void bindTexture();
    void load_file(char* filename);
	static unsigned char* BuildText(const char* filename, int width, int height);
	void Load_file_rast(unsigned char* image_buffer, int width, int height);
	TextureResource();
	~TextureResource();

};
