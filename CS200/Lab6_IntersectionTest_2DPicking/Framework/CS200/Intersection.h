/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#pragma once
#include "Math.h"
#include "CS200/Rectangle.h"
#include "CS200/Circle.hpp"

namespace CS200
{

bool PointVSAABB(const Point& p, const Rectangle& rect);
bool PointVSCircle(const Point& p, const Circle& circle);
bool CircleVSCircle(const Circle& circle1, const Circle& circle2);
bool AABBVSAABB(const Rectangle& rect1, const Rectangle& rect2);
float ShortestDistanceSqr(const Point& p, const Edge& edge);
bool CircleVSAABB(const Circle& circle, const Rectangle& rect);

}
