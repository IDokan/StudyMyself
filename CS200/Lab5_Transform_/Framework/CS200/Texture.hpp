/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include <cassert>
#include <vector>
#include <iostream>
#include "CS200/Color.hpp"
#include "external/stb/include/stb_image.h"

namespace CS200
{

class Texture
{
public:
	Texture(const std::string& file_path)
		: width(0), height(0), comps(0), channel_count(0)
	{
		unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &comps, 4);
		assert(data);

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				unsigned char* pixel = data + (x + height * y) * 4;
				colors.emplace_back(pixel[0], pixel[1], pixel[2], pixel[3]);
			}
		}

		stbi_image_free(data);
	}

	const Color& GetTextureColor(int x, int y) const
	{
		assert(x >= 0 && x < width);
		assert(y >= 0 && y < height);
		return colors[x + y * width];
	}

	int GetTextureWidth() const { return width; }
	int GetTextureHeight() const { return height; }

private:
	int width;
	int height;
	int comps;
	int channel_count;
	std::vector<Color> colors;
};

}