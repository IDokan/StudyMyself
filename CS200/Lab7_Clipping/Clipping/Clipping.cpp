/*
 * sinil.gang
 * CS200 Lab 7 - Clipping
 * CS200
 * Fall 2019
 */
#include "CS200/Clipping.h"

/* Helper functions */
void ClipPolygonWithGivenEdge(Point start, Point end, std::vector<Point>& result, const ViewportCoord& viewport, Boundary boundary);
void ClipPolygonWithGivenBoundary(std::vector<Point>& vertices, const ViewportCoord& viewport, Boundary boundary);
/* End of helper functions */

void ClipPolygonWithGivenEdge(Point start,Point end, std::vector<Point>& result, const ViewportCoord& viewport, Boundary boundary)
{
	Edge edge{ start, end };
	Point intersectionPoint{};

	// Get intersected edge type
	const EdgeType intersectionResult = edge.GetIntersection(intersectionPoint, boundary, viewport);

	// Do an appropriate calculation based on given intersected edge type
	switch (intersectionResult)
	{
	case EdgeType::IN_TO_IN:
		result.push_back(end);
		break;
	case EdgeType::IN_TO_OUT:
		result.push_back(intersectionPoint);
		break;
	case EdgeType::OUT_TO_OUT:
		// Save none
		break;
	case EdgeType::OUT_TO_IN:
		result.push_back(intersectionPoint);
		result.push_back(end);
		break;

	case EdgeType::ERROR:
	default:;
	}
}

void ClipPolygonWithGivenBoundary(std::vector<Point>& vertices, const ViewportCoord& viewport, Boundary boundary)
{
	std::vector<Point> result{};
	// Walk though every edge
	for (size_t index = 0; index < vertices.size() - 1; ++index)
	{
		ClipPolygonWithGivenEdge(vertices.at(index), vertices.at(index+1), result, viewport, boundary);
	}
	// Last clip for edge between last and first vertex
	ClipPolygonWithGivenEdge(vertices.back(), vertices.front(), result, viewport, boundary);

	// Update vertices
	vertices = result;
}

void Clip(std::vector<Point>& vertices, const ViewportCoord& viewport)
{
	// Initialize needed variable
	constexpr Boundary boundaries[] = { Boundary::Left, Boundary::Right, Boundary::Up, Boundary::Down };
	for (const Boundary& boundary : boundaries)
	{
		ClipPolygonWithGivenBoundary(vertices, viewport, boundary);
	}
}