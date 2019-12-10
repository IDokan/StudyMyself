#pragma once

struct Vector
{
	Vector(float x = 0, float y = 0) : x(x), y(y) {}

	static float dot(const Vector& vec1, const Vector& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	float x;
	float y;
};

struct Point
{
	Point(float x = 0, float y = 0) : x(x), y(y) {}

	Point operator+(const Point& rhs) const
	{
		return Point(x + rhs.x, y + rhs.y);
	}

	Point operator-(const Point& rhs) const
	{
		return Point(x - rhs.x, y - rhs.y);
	}

	bool operator==(const Point& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	friend Point operator*(float t, const Point & point)
	{
		return Point(t * point.x, t * point.y);
	}

	float x;
	float y;
};
