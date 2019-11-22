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
		return (pow(p.x, 2) + pow(p.y, 2));
	}
}

namespace CS200
{
	////////////
	//2D picking
	bool PointVSAABB(const Point& p, const Rectangle& rect)
	{
		Point max = rect.GetMax();
		Point min = rect.GetMin();
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
		int sumRadius = circle1.GetRadius() + circle2.GetRadius();
		Vector distance = circle1.GetCenter() - circle2.GetCenter();
		return Helper::MagnitudeSquared(sumRadius) >= Helper::MagnitudeSquared(distance);
	}

	bool AABBVSAABB(const Rectangle& rect1, const Rectangle& rect2)
	{
		Point rect1Min = rect1.GetMin();
		Point rect2Min = rect2.GetMin();
		Point rect1Max = rect1.GetMax();
		Point rect2Max= rect2.GetMax();
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
		Vector RP{p - edge.start};					/* R - P */
		Vector QP{ edge.end - edge.start };	/* Q - P */
		if (Vector::dot(RP, QP) < 0)
		{
			return Helper::MagnitudeSquared(RP);
		}
		// CASE 2
		else if (Vector::dot(RP, QP) > Vector::dot(QP, QP))
		{
			Vector RQ{ p - edge.end };
			return Helper::MagnitudeSquared(RP);
		}
		// CASE 3
		else if(float sp = Vector::dot(RP, QP); 
			0 <= sp && sp <= Helper::MagnitudeSquared(QP))
		{
			return sp;
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
			// Compute shortest distance of circle center from AABB boundary
			
			if (ShortestDistanceSqr(circle.GetCenter(rect.GetTopLeft() - rect.Get), /**/)<= circle.GetRadius())
			{
				
			}
		}	
		return false;
	}
}