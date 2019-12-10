#include <CS200/Image.hpp>
#include <cstdlib>
#include "CS200/Line.h"
#include "CS200/Clipping.h"

using namespace CS200;

const Color BLACK{0x000000FF};
const Color GREY{ 0xF0F0F0FF };
const Color WHITE{0xFFFFFFFF};
const Color JAGUAR_BLACK{0x1F0322FF};
const Color DISCO_PURPLE{0x8a1c7cFF};
const Color CABARET_RED{0xda4167FF};
const Color BATTLESHIP_GRAY{0x899d78FF};

const int WIDTH = 512;
const int HEIGHT = 512;
const Color BEAUTY_BUSH_PINK{0xf0bcd4FF};
Image gImage{WIDTH, HEIGHT, WHITE };

int ROUND(double f) { return static_cast<int>(floor(f + 0.5f)); }

int main(void)
{
	//Grey Viewport
	int viewport_min_x = WIDTH / 4;
	int viewport_min_y = HEIGHT / 4;
	ViewportCoord viewport(viewport_min_x, viewport_min_x * 3, viewport_min_y, viewport_min_y * 3);
	for (int y = viewport.min_y; y <= viewport.max_y; ++y)
	{
		for (int x = viewport.min_x; x <= viewport.max_x; ++x)
			gImage.SetPixel(x, y, GREY);
	}

	//Points
	Point v1(64, 30);
	Point v2(300, 450);
	Point v3(491, 400);
	std::vector<Point> points;
	points.push_back(v1);
	points.push_back(v2);
	points.push_back(v3);

	//Clipping
	Clip(points, viewport);

	//Edge Drawing
	int point_num = static_cast<int>(points.size());
	for (int i = 0; i < point_num; ++i)
	{
		int next_index = (i + 1) == point_num ? 0 : i + 1;
		DrawLine(
			static_cast<int>(points[i].x),
			static_cast<int>(points[i].y),
			static_cast<int>(points[next_index].x),
			static_cast<int>(points[next_index].y),
			BLACK, gImage);
	}

    gImage.SaveToPNG("results.png");
    system("start results.png");
    return 0;
}

