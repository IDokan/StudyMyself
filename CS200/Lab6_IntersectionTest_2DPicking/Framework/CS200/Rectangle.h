/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include "Color.hpp"
#include "Image.hpp"
#include "Math.h"
#include "Triangle.h"

namespace CS200
{

class Rectangle
{
public:
	Rectangle(const Point& top_left, const Point& bottom_right, const Color& color)
		: top_left(top_left), bottom_right(bottom_right), color(color) {}

	Point GetMin() const { return Point(top_left.x, bottom_right.y); } // bottom-left
	Point GetMax() const { return Point(bottom_right.x, top_left.y); } // top-right

	const Point& GetTopLeft() const { return top_left; }
	const Point& GetBottomRight() const { return bottom_right; }
	void SetColor(const Color& col) { color = col; }

	void Draw(Image& image)
	{
		DrawTriangle(top_left, bottom_right, Point(top_left.x, bottom_right.y), color, image);
		DrawTriangle(top_left, bottom_right, Point(bottom_right.x, top_left.y), color, image);
	}

private:
	Point top_left;
	Point bottom_right;
	Color color;
};

}
