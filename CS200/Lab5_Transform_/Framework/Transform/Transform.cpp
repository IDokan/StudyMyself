/*
 * sinil.gang
 * Lab 5 - 2D Transformation
 * CS200
 * Fall 2019
 */
#include "Transform.h"

namespace
{
	constexpr float PI = 3.141592f;
	constexpr float PIinDegree = 180.f;
}

Matrix WorldToRelative(const Vector& u, const Vector& v, const Point& C)
{
	const Vector uNor = Normalize(u);
	const Vector vNor = Normalize(v);
	const Vector position(C.x, C.y);
	return Matrix(
		Vector(uNor.x, vNor.x),
		Vector(uNor.y, vNor.y),
		Point(-Dot(position, uNor), -Dot(position, vNor))
	);
}

Matrix RelativeToWorld(const Vector& u, const Vector& v, const Point& C)
{
	const Vector uNor = Normalize(u);
	const Vector vNor = Normalize(v);
	return Matrix(
		uNor, vNor, C
	);
}

Matrix Scale(const Vector& scale)
{
	return Matrix(
		Vector(scale.x),
		Vector(0, scale.y),
		Point(0, 0)
	);
}

Matrix Rotate(int degree)
{
	const float cosValue = static_cast<float>(cos(degree / PIinDegree * PI));
	const float sinValue = static_cast<float>(sin(degree / PIinDegree * PI));
	return Matrix(
		Vector(cosValue, sinValue),
		Vector(-sinValue, cosValue),
		Point(0, 0)
	);
}

Matrix Translate(const Vector& translate)
{
	return Matrix(
		Vector(1),
		Vector(0, 1),
		Point(translate.x, translate.y)
	);
}
