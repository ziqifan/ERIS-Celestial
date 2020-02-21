#include "ImageLoader.h"
Texture2D ImageLoader::loadImage2D(const char * path)
{
	Texture2D texture;
	//int width, height;

	unsigned char *image = SOIL_load_image(path, &texture.width, &texture.height, nullptr, SOIL_LOAD_RGBA);

	if(image == nullptr)
	{
		printf("Image \"%s\", returned with null pointer\n", path);
		return texture;
	}

	//Bind texture to model
	glGenTextures(1, &texture.id);
	texture.bindTexture();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	texture.unbindTexture();


	//texture.width = width;
	//texture.height = height;
	return texture;
}

#include <vector>
#include <fstream>
Texture3D ImageLoader::loadImage3D(const char * LUTpath)
{
	Texture3D texture;
	/////////////////////////////////////Bind Custom 3D Texture////////////////////////////////////////////
	std::vector<Coord3D> LUT{};
	//std::string LUTpath = "";

	//LUTpath = "Texture/CUSTOM.cube";
	std::ifstream LUTfile2(LUTpath);

	while(!LUTfile2.eof())
	{
		std::string LUTline;
		getline(LUTfile2, LUTline);
		if(LUTline.empty()) continue;
		if(strstr(LUTline.c_str(), "LUT_3D_SIZE"))
		{
			sscanf_s(LUTline.c_str(), "LUT_3D_SIZE %d", &texture.lutSize);
		}
		float r, g, b;
		if(sscanf_s(LUTline.c_str(), "%f %f %f", &r, &g, &b) == 3) LUT.push_back({r,g,b});
	}
	glEnable(GL_TEXTURE_3D);

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_3D, texture.id);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, texture.lutSize, texture.lutSize, texture.lutSize, 0, GL_RGB, GL_FLOAT, LUT.data());

	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	LUT.clear();
	////////////////////////////////////////////////////////////////////////////////////////////////////
	return texture;
}
