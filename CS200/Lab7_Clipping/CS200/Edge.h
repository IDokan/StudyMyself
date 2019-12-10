#pragma once
#include "Math.h"

enum class Boundary
{
	Left,	// Left edge of viewport
	Right,	// Right edge of viewport
	Up,		// Top edge of viewport	
	Down	// Bottom edge of viewport
};

enum class EdgeType
{
	IN_TO_IN,
	IN_TO_OUT,
	OUT_TO_OUT,
	OUT_TO_IN,
	ERROR
};

struct ViewportCoord
{
	ViewportCoord(int min_x = 0, int max_x = 0, int min_y = 0, int max_y = 0)
		: min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y) {}

	/*
								
							//////////////////////	-> (max_x, max_y)
							//					//
							//		viewport	//
							//					//
							//					//
		(min_x, min_y) <-	//////////////////////

	*/

	int min_x;
	int max_x;
	int min_y;
	int max_y;
};

class Edge
{
public:
	Edge(const Point & start, const Point & end);

	/******************************************************************************
	*Function Name : GetIntersection
	*Purpose : 
		Gets intersecting point of two edges (*this edge vs Boundary of viewport)

	*Parameters:
		- intersection : Empty point value (provided by users) 
						where the intersecting point is stored
		- boundary : Boundary of viewport which is needed for getting intersecting point
		- viewport : Viewport struct which is provided by framework

	*Usage:

		Point start(?, ?);
		Point end(?, ?);
		Edge edge(start, end);

		Point intersection;
		EdgeType edge_type = edge.GetIntersection(intersection, Boundary::Left, viewport);

		//Check the edge_type and use 'intersection' value...


	*Return value:
		EdgeType::IN_TO_IN - means that start/end points are both inside the viewport,
							'intersection' value is still empty

		EdgeType::IN_TO_OUT - means that start point is inside and end point is outside,
							intersecting point is stored to 'intersection' value

		EdgeType::OUT_TO_OUT - means that start/end points are both outside the viewport,
							'intersection' value is still empty

		EdgeType::OUT_TO_IN - means that start point is outside and end point is inside,
							intersecting point is stored to 'intersection' value

	******************************************************************************/
	EdgeType GetIntersection(Point& intersection, Boundary boundary, 
		const ViewportCoord & viewport);

private:
	struct HalfSpace2D
	{
		HalfSpace2D(const Vector& normal, const Point & point)
			: a(normal.x), b(normal.y),
			c(-(a * point.x + b * point.y)) {}

		float operator*(const Point & point) const
		{
			return a * point.x + b * point.y + c;
		}

		float a = 0;
		float b = 0;
		float c = 0;
	};

	EdgeType GetIntersection(Point& intersection, const HalfSpace2D& half_space);

	Point start;
	Point end;
};