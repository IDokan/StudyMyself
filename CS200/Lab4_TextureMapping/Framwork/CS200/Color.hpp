/*
 * Rudy Castan
 * Graphics Library
 * CS200
 * Fall 2018
 */
#pragma once

namespace CS200
{
    struct Color
    {
    public:
        using ColorChannel = unsigned char;
        using RGBA32 = unsigned int;

        ColorChannel Red = 0;
        ColorChannel Green = 0;
        ColorChannel Blue = 0;
        ColorChannel Alpha = 255;
    	
    public:
        Color() = default;
        Color(ColorChannel red, ColorChannel green, ColorChannel blue, ColorChannel alpha = 255);
        explicit Color(RGBA32 color);
    };

    static_assert(sizeof(Color) == sizeof(Color::RGBA32),
                  "Color should be the same as 4 bytes, so we can easily treat it as an RGBA int.");
}


namespace CS200
{
    inline Color::Color(ColorChannel red, ColorChannel green, ColorChannel blue, ColorChannel alpha)
        : Red(red), Green(green), Blue(blue), Alpha(alpha)
    {
    }

    inline Color::Color(RGBA32 color)
        : Red(static_cast<ColorChannel>((color & 0xff000000) >> 24)),
          Green(static_cast<ColorChannel>((color & 0x00ff0000) >> 16)),
          Blue(static_cast<ColorChannel>((color & 0x0000ff00) >> 8)),
          Alpha(static_cast<ColorChannel>((color & 0x000000ff) >> 0))
    {
    }
}
