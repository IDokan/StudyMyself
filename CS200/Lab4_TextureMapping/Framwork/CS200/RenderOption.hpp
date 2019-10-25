/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include "Texture.hpp"

namespace CS200
{

enum class TextureFunction
{
	REPEAT,
	MIRROR,
	CLAMP
};

enum class ColorCombineMode
{
	DECAL,
	MODULATE,
	REPLACE
};

struct RenderOption
{
	Texture* texture;
	TextureFunction texture_func;
	ColorCombineMode cc_mode;
};

}

