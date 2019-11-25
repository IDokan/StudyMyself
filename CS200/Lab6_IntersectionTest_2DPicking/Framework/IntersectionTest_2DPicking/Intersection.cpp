/*
 * sinil.gang
 * Lab 6 - Intersection Test & 2D Picking Framework
 * CS200
 * Fall 2019
 */
#include "CS200/Intersection.h"

namespace Helper
{
	float MagnitudeSquared(const Vector& p)
	{
		return static_cast<float>((pow(p.x, 2) + pow(p.y, 2)));
	}
}

namespace CS200
{
	////////////
	//2D picking
	bool PointVSAABB(const Point& p, const Rectangle& rect)
	{
		const Point max = rect.GetMax();
		const Point min = rect.GetMin();
		return (p.x >= min.x) &&
			(p.x <= max.x) &&
			(p.y >= min.y) &&
			(p.y <= max.y);
	}

	bool PointVSCircle(const Point& p, const Circle& circle)
	{
		const Vector distance = p - circle.GetCenter();
		return Helper::MagnitudeSquared(distance) <= pow(circle.GetRadius(), 2);
	}

	//////////////
	//Intersection
	bool CircleVSCircle(const Circle& circle1, const Circle& circle2)
	{
		const int sumRadius = circle1.GetRadius() + circle2.GetRadius();
		const Vector distance = circle1.GetCenter() - circle2.GetCenter();
		return Helper::MagnitudeSquared(sumRadius) >= Helper::MagnitudeSquared(distance);
	}

	bool AABBVSAABB(const Rectangle& rect1, const Rectangle& rect2)
	{
		const Point rect1Min = rect1.GetMin();
		const Point rect2Min = rect2.GetMin();
		const Point rect1Max = rect1.GetMax();
		const Point rect2Max= rect2.GetMax();
		return !(
			(rect1Max.x < rect2Min.x) ||
			(rect2Max.x < rect1Min.x) ||
			(rect1Max.y < rect2Min.y) ||
			(rect2Max.y < rect1Min.y)
			);
	}

	//Helper function for CircleVSAABB
	float ShortestDistanceSqr(const Point& p, const Edge& edge)
	{
		// CASE 1
		const Vector targetPointVector{p - edge.start};					/* R - P */
		const Vector lineVector{ edge.end - edge.start };					/* Q - P */
		if (Vector::dot(targetPointVector, lineVector) < 0)
		{
			return targetPointVector.Length();
		}
		// CASE 2
		else if (Vector::dot(targetPointVector, lineVector) > Vector::dot(lineVector, lineVector))
		{
			const Vector targetPointWithEndVector{ p - edge.end };	/* R - Q */
			return targetPointWithEndVector.Length();
		}
		// CASE 3
		else if(const float measure = Vector::dot(targetPointVector, lineVector); 
			0 <= measure && measure <= Helper::MagnitudeSquared(lineVector))
		{
			return sqrt(Helper::MagnitudeSquared(targetPointVector) - (sqrt(Vector::dot(targetPointVector, lineVector))/Helper::MagnitudeSquared(lineVector)));
		}
		else
		{
			// Unexpected situation
			return 0.f;
		}
	}

	bool CircleVSAABB(const Circle& circle, const Rectangle& rect)
	{
		if (PointVSAABB(circle.GetCenter(), rect))
		{
			return true;
		}
		else
		{
			float floatRadius = static_cast<float>(circle.GetRadius());
			// Compute shortest distance of circle center from AABB boundary
			if (ShortestDistanceSqr(circle.GetCenter(), Edge(rect.GetTopLeft(), rect.GetMax())) <= floatRadius)
			{
				return true;
			}
			if (ShortestDistanceSqr(circle.GetCenter(), Edge(rect.GetTopLeft(), rect.GetMin())) <= floatRadius)
			{
				return true;
			}
			if (ShortestDistanceSqr(circle.GetCenter(), Edge(rect.GetBottomRight(), rect.GetMax())) <= floatRadius)
			{
				return true;
			}
			if (ShortestDistanceSqr(circle.GetCenter(), Edge(rect.GetBottomRight(), rect.GetMin())) <= floatRadius)
			{
				return true;
			}
		}	
		return false;
	}
}