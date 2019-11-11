/*
 * Jooho Jeong
 * Lab 5 framework
 * CS200
 * Fall 2019
 */
#include "CS200\Object.h"
#include "Transform.h"
#include "CS200/Triangle.h"

Object::Object(const CS200::Texture& texture , const Point & position)
	: m_texture(texture), m_position(position)
{
	float tex_half_width = texture.GetTextureWidth() / 2.f;
	float tex_half_height = texture.GetTextureHeight() / 2.f;

	//top left
	m_vertices[0] = Point(-tex_half_width, tex_half_height);
	//bottom left 
	m_vertices[1] = Point(-tex_half_width, -tex_half_height);
	//bottom right
	m_vertices[2] = Point(tex_half_width, -tex_half_height);
	//top right
	m_vertices[3] = Point(tex_half_width, tex_half_height);
}

void Object::Rotate(int degree)
{
	m_relative_y_axis = ::Rotate(degree) * m_relative_y_axis;
	m_degree = degree;
}

void Object::Draw(CS200::Image & image, const Matrix& world_to_relative, const Matrix& viewport_transform) const
{
	Matrix model_to_viewport = viewport_transform * world_to_relative * GetModelTransform();

	//Vertices
	Point p1 = model_to_viewport * m_vertices[0];
	Point p2 = model_to_viewport * m_vertices[1];
	Point p3 = model_to_viewport * m_vertices[2];
	Point p4 = model_to_viewport * m_vertices[3];

	Vertex2D v1(p1, 0, 0); // top left
	Vertex2D v2(p2, 0, 1); // bottom left
	Vertex2D v3(p3, 1, 1); // bottom right
	Vertex2D v4(p4, 1, 0); // top right

	DrawTriangle(v1, v3, v2, m_texture, image);
	DrawTriangle(v1, v4, v3, m_texture, image);
}

Matrix Object::WorldToRelative() const
{
	return ::WorldToRelative(::Rotate(-90) * m_relative_y_axis, m_relative_y_axis, m_position);
}

Matrix Object::RelativeToWorld() const
{
	return ::RelativeToWorld(::Rotate(-90) * m_relative_y_axis, m_relative_y_axis, m_position);
}

Matrix Object::GetModelTransform() const
{
	return Translate(Vector(m_position.x, m_position.y)) * ::Rotate(m_degree) * Scale(m_scale);
}
