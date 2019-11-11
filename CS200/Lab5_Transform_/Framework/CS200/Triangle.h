/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include "Color.hpp"
#include "Texture.hpp"
#include "Transform/Math.h"
#include "Image.hpp"

struct Vector3D
{
	Vector3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	static float dot(const Vector3D& vec1, const Vector3D& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	static Vector3D cross(const Vector3D& vec1, const Vector3D& vec2)
	{
		return Vector3D(vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x);
	}

	float x;
	float y;
	float z;
};

struct Vertex2D
{
	Vertex2D(float x, float y, float u, float v)
		: x(x), y(y), u(u), v(v) {}
	Vertex2D(const Hcoord& p, float u, float v)
		: x(p.x), y(p.y), u(u), v(v) {}

	Vector3D operator-(const Vertex2D& vertex) const
	{
		return Vector3D(x - vertex.x, y - vertex.y, 0);
	}

	float x = 0;
	float y = 0;
	float u = 0;
	float v = 0;
};

void DrawTriangle(const Vertex2D& v1, const Vertex2D& v2, const Vertex2D& v3, 
	const CS200::Texture& texture, CS200::Image & image);