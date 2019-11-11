/*
 * Jooho Jeong
 * Lab 5 framework
 * CS200
 * Fall 2019
 */
#pragma once
#include "Texture.hpp"
#include "Transform/Math.h"
#include "CS200/Image.hpp"

class Object
{
public:
	Object(const CS200::Texture& texture, const Point & position);

	void SetPosition(const Point& pos) { m_position = pos; }
	void SetScale(const Vector& scale) { m_scale = scale; }
	void Rotate(int degree);

	void Draw(CS200::Image & image, const Matrix& world_to_relative, const Matrix& viewport_transform) const;
	Matrix WorldToRelative() const;
	Matrix RelativeToWorld() const;

private:
	Matrix GetModelTransform() const;

	const CS200::Texture& m_texture;
	Point m_vertices[4];

	Point m_position;
	Vector m_scale = Vector(1, 1);

	Vector m_relative_y_axis = Vector(0, 1);
	int m_degree = 0;
};