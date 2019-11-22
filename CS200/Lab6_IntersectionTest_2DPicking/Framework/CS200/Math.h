/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include <cmath>

struct Vector
{
	Vector(int x = 0, int y = 0) : x(x), y(y) {}

	static float dot(const Vector& vec1, const Vector& vec2)
	{
		return static_cast<float>(vec1.x * vec2.x + vec1.y * vec2.y);
	}

	float Length() const
	{
		return static_cast<float>(sqrt(x * x + y * y));
	}

	int x;
	int y;
};

struct Point
{
	Point(int x = 0, int y = 0) : x(x), y(y) {}

	Vector operator-(const Point& rhs) const
	{
		return Vector(x - rhs.x, y - rhs.y);
	}

	int x;
	int y;
};

struct Edge
{
	Edge(const Point & start, const Point & end) : start(start), end(end) {}

	Point start;
	Point end;
};
