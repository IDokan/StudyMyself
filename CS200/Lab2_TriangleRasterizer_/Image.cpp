/*
 * Rudy Castan
 * Graphics Library
 * CS200
 * Fall 2018
 */
#include "CS200/Image.hpp"
#include <cassert>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include <stb_image_write.h>

namespace CS200
{
    Image::Image(int pixel_width, int pixel_height, Color color)
        : width(pixel_width), height(pixel_height), pixels(pixel_width * pixel_height, color)
    {
    }    

    void Image::SaveToPNG(const std::string& file_path) const
    {
        if (pixels.empty())
            return;
        const int stride = 0;
        stbi_write_png(file_path.c_str(), width, height, ChannelsPerColor, &pixels[0], stride);
    }

    void Image::SetPixel(int column, int row, Color color)
    {
        assert(column >= 0 && column < width);
        assert(row >= 0 && row < height);
        pixels[column + row * width] = color;
    }
}
