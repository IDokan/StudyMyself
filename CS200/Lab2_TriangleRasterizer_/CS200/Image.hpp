/*
 * Rudy Castan
 * Graphics Library
 * CS200
 * Fall 2018
 */
#pragma once
#include "Color.hpp"
#include <string>
#include <vector>

namespace CS200
{
    class Image
    {
    public:
        explicit Image(int pixel_width = 0, int pixel_height = 0, Color color = Color{});
        void SaveToPNG(const std::string& file_path) const;
        void SetPixel(int column, int row, Color color);

    private:
        int width = 0;
        int height = 0;
        std::vector<Color> pixels{};

        static const int ChannelsPerColor = sizeof(Color::RGBA32);
    };
}
