﻿/*	1. sinil.gang
 *	2. Lab2: Triangle Rasterizer
 *	3. CS200
 *	4. Fall 2019
 */

#include <CS200/Image.hpp>
#include <cstdlib>

using namespace CS200;

const Color BLACK{ 0x000000FF };
const Color WHITE{ 0xFFFFFFFF };
const Color JAGUAR_BLACK{ 0x1F0322FF };
const Color DISCO_PURPLE{ 0x8a1c7cFF };
const Color CABARET_RED{ 0xda4167FF };
const Color BATTLESHIP_GRAY{ 0x899d78FF };

const int WIDTH = 256;
const int HEIGHT = 256;
const Color BEAUTY_BUSH_PINK{ 0xf0bcd4FF };
Image gImage{ WIDTH, HEIGHT, BEAUTY_BUSH_PINK };

struct Vector
{
    Vector(int x = 0, int y = 0) : x(x), y(y) {}

    static int dot(const Vector & vec1, const Vector & vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    int x;
    int y;
};

struct Point
{
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    Vector operator-(const Point & rhs) const
    {
        return Vector(x - rhs.x, y - rhs.y);
    }
	int operator*(const Vector& v) const
    {
		return x * v.x + y * v.y;
    }

    int x;
    int y;
};

int ROUND(double f) { return static_cast<int>(floor(f + 0.5)); }

void SETPIXEL(int column, int row, Color intensity) { gImage.SetPixel(column, row, intensity); }

void DrawTriangle(const Point & p1, const Point & p2, const Point & p3, Color intensity);

int main(void)
{
    Point p1(128, 10);
    Point p2(246, 246);
    Point p3(10, 246);
    DrawTriangle(p1, p2, p3, DISCO_PURPLE);

    gImage.SaveToPNG("results.png");
    system("start results.png");
    return 0;
}

bool AreTheyAllInside(int* decisions)
{
	return (decisions[0] >= 0) &&
		(decisions[1] >= 0) &&
		(decisions[2] >= 0);
}

void InitialArray(int* arr, int* arr2, const int size)
{
	for (int i = 0; i< size; ++i)
	{
		arr[i] = arr2[i];
	}
}

Point GetSmallPoint(const Point& p1, const Point& p2, const Point& p3)
{
	Point result;
	result.x = (p1.x < p2.x) ? p1.x : p2.x;
	result.x = (result.x < p3.x) ? result.x : p3.x;
	result.y = (p1.y < p2.y) ? p1.y : p2.y;
	result.y = (result.y < p3.y) ? result.y : p3.y;

	return result;
}

Point GetLargePoint(const Point& p1, const Point& p2, const Point& p3)
{
	Point result;
	result.x = (p1.x > p2.x) ? p1.x : p2.x;
	result.x = (result.x > p3.x) ? result.x : p3.x;
	result.y = (p1.y > p2.y) ? p1.y : p2.y;
	result.y = (result.y > p3.y) ? result.y : p3.y;

	return result;
}

void DrawTriangle(const Point & p1, const Point & p2,
    const Point & p3, Color intensity)
{
	const Point smallPoint = GetSmallPoint(p1, p2, p3);
	const Point largePoint = GetLargePoint(p1, p2, p3);
	
	// Given Lines
	const Vector line1{ p3 - p1 };
	const Vector line2{ p2 - p3 };
	const Vector line3{ p1 - p2 };

	// Normal vectors
	const Vector nLine1{ line1.y, - line1.x };
	const Vector nLine2{ line2.y, - line2.x };
	const Vector nLine3{ line3.y, - line3.x };
	
	const int size = 3;
	int startingDecision[size]
	{
		smallPoint * nLine1 - p3 * nLine1,
		smallPoint * nLine2 - p2 * nLine2,
		smallPoint * nLine3 - p1 * nLine3
	};
	int tempDecision[size]{ startingDecision[0], startingDecision[1], startingDecision[2] };

	for (int row = smallPoint.y; row < largePoint.y; ++row)
	{
		// Initialize tempDecision as initial value of starting decision.
		InitialArray(tempDecision, startingDecision, size);
		
		for (int column = smallPoint.x; column < largePoint.x; ++column)
		{
			if (AreTheyAllInside(tempDecision))
			{
				SETPIXEL(column, row, intensity);
			}
			// Add X value
			tempDecision[0] += (nLine1.x);
			tempDecision[1] += (nLine2.x);
			tempDecision[2] += (nLine3.x);
		}
		// Add Y value
		startingDecision[0] += (nLine1.y);
		startingDecision[1] += (nLine2.y);
		startingDecision[2] += (nLine3.y);
	}
}