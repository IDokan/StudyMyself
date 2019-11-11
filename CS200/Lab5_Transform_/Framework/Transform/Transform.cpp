#include "Transform.h"

namespace
{
	constexpr float PI = 3.141592f;
	constexpr float PIinDegree = 180.f;
}

// Note that u and v is not guaranteed if they are already normalized.
Matrix WorldToRelative(const Vector& u, const Vector& v, const Point& C)
{
	Vector uNor = Normalize(u);
	Vector vNor = Normalize(v);
	return Matrix(
		Hcoord(uNor.x, vNor.x, 0),
		Hcoord(uNor.y, vNor.y, 0),
		C
	);
}

Matrix RelativeToWorld(const Vector& u, const Vector& v, const Point& C)
{
	Vector uNor = Normalize(u);
	Vector vNor = Normalize(v);
	Vector position(C.x, C.y);
	return Matrix(
		uNor,
		vNor,
		Point(Dot(position, uNor), Dot(position, vNor))
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
