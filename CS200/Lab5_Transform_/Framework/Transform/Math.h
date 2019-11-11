/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include <iostream>
#include <cmath>
#include <cassert>

struct Hcoord
{
	Hcoord(float x = 0, float y = 0, float w = 0) : x(x), y(y), w(w) {}
	float& operator[](int i)
	{
		assert(i >= 0 && i <= 2);
		return *(&x + i);
	}
	float operator[](int i) const
	{
		assert(i >= 0 && i <= 2);
		return *(&x + i); 
	}
	float x, y, w;
};

struct Point : public Hcoord
{
	Point(const Hcoord& hcoord) : Hcoord(hcoord) { assert(hcoord.w == 1); }
	Point(float x = 0, float y = 0) : Hcoord(x, y, 1) {}

	void operator=(const Hcoord& hcoord)
	{
		assert(hcoord.w == 1);
		x = hcoord.x;
		y = hcoord.y;
		w = hcoord.w;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point& point)
	{
		os << "(" << point.x << ", " << point.y << ")";
		return os;
	}
};

struct Vector : public Hcoord
{
	Vector(const Hcoord& hcoord) : Hcoord(hcoord) { assert(hcoord.w == 0); }
	Vector(float x = 0, float y = 0) : Hcoord(x, y, 0) {}
};

struct Matrix
{
	Matrix()
	{
		row[2].w = 1;
	}

	Matrix(const Hcoord& vec1, const Hcoord& vec2, const Hcoord& point)
	{
		for (int i = 0; i < 3; ++i)
		{
			row[i].x = vec1[i];
			row[i].y = vec2[i];
			row[i].w = point[i];
		}
	}

	Hcoord& operator[](int i)
	{
		assert(i >= 0 && i <= 2);
		return row[i];
	}
	const Hcoord& operator[](int i) const
	{
		assert(i >= 0 && i <= 2);
		return row[i];
	}

	Hcoord row[3];
};

inline Vector Normalize(const Vector& vector)
{
	float mag = sqrt(vector.x * vector.x + vector.y * vector.y);
	return Vector(vector.x / mag, vector.y / mag);
}

inline float Dot(const Vector& vec1, const Vector& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

inline Hcoord operator*(const Matrix& mat, const Hcoord& hcoord)
{
	Hcoord result;
	for (int i = 0; i < 3; ++i)
	{
		result[i] += mat[i].x * hcoord.x;
		result[i] += mat[i].y * hcoord.y;
		result[i] += mat[i].w * hcoord.w;
	}
	return result;
}

inline Matrix operator*(const Matrix& mat1, const Matrix& mat2)
{
	Hcoord mat2_col1(mat2[0].x, mat2[1].x, mat2[2].x);
	Hcoord mat2_col2(mat2[0].y, mat2[1].y, mat2[2].y);
	Hcoord mat2_col3(mat2[0].w, mat2[1].w, mat2[2].w);

	return Matrix(mat1 * mat2_col1, mat1 * mat2_col2, mat1 * mat2_col3);
}