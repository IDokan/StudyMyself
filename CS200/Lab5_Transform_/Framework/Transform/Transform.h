/*
 * Jooho Jeong
 * Lab 5 framework
 * CS200
 * Fall 2019
 */
#pragma once
#include "Math.h"

Matrix WorldToRelative(const Vector & u, const Vector & v, const Point & C);
Matrix RelativeToWorld(const Vector & u, const Vector & v, const Point & C);

Matrix Scale(const Vector& scale);
Matrix Rotate(int degree);
Matrix Translate(const Vector& translate);