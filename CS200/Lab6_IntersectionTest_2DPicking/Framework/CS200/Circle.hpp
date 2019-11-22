/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include "Image.hpp"
#include "Color.hpp"
#include "Math.h"

namespace CS200
{

class Circle
{
public:
	Circle(int x, int y, int r, const Color & color = Color(0, 0, 0, 255))
		: center_x(x), center_y(y), radius(r), color(color) {}

	Point GetCenter() const { return Point(center_x, center_y); }
	int GetCenterX() const { return center_x; }
	int GetCenterY() const { return center_y; }

	int GetRadius() const { return radius; }

	void SetColor(const Color& c) { color = c; }
	void Draw(Image & gImage);

private:
	int center_x, center_y;
	int radius;
	Color color;
};

}
