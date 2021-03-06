#include <CS200/Image.hpp>
#include <cstdlib>
#include <algorithm>

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


void SETPIXEL(int column, int row, Color intensity) { gImage.SetPixel(column, row, intensity); }

// Draw axis align line
void DrawFixedLine(int a0, int a1, int b, Color intensity, bool isHorizontal = true)
{
	if (isHorizontal)
	{
		for (int a = a0; a <= a1; ++a)
		{
			SETPIXEL(a, b, intensity);
		}
	}
	else
	{
		for (int a = a0; a <= a1 + 1; ++a)
		{
			SETPIXEL(b, a, intensity);
		}
	}
}

void DrawLine(int x0, int y0, int x1, int y1, Color intensity);

int main(void)
{
	//DrawLine(10, 50, 246, 206, BLACK);
	//gImage.SaveToPNG("test0.png");

	//gImage.Clear(BEAUTY_BUSH_PINK);
	//DrawLine(246, 206, 10, 50, BLACK);
	//gImage.SaveToPNG("test1.png");

	gImage.Clear(BEAUTY_BUSH_PINK);
	DrawLine(50, 10, 206, 246, BLACK);
	gImage.SaveToPNG("test2.png");

	gImage.Clear(BEAUTY_BUSH_PINK);
	DrawLine(206, 246, 50, 10, BLACK);
	gImage.SaveToPNG("test3.png");

	//gImage.Clear(BEAUTY_BUSH_PINK);
	//DrawLine(50, 246, 206, 10, BLACK);
	//gImage.SaveToPNG("test4.png");

	//gImage.Clear(BEAUTY_BUSH_PINK);
	//DrawLine(206, 10, 50, 246, BLACK);
	//gImage.SaveToPNG("test5.png");

	//gImage.Clear(BEAUTY_BUSH_PINK);
	//DrawLine(10, 206, 246, 50, BLACK);
	//gImage.SaveToPNG("test6.png");

	//gImage.Clear(BEAUTY_BUSH_PINK);
	//DrawLine(246, 50, 10, 206, BLACK);
	//gImage.SaveToPNG("test7.png");

	//gImage.Clear(BEAUTY_BUSH_PINK);
	//DrawLine(10, 128, 246, 128, BLACK);
	//gImage.SaveToPNG("test8.png");

	//gImage.Clear(BEAUTY_BUSH_PINK);
	//DrawLine(128, 10, 128, 248, BLACK);
	//gImage.SaveToPNG("test9.png");

	return 0;
}

void DrawLine(int x0, int y0, int x1, int y1, Color intensity)
{
	if (x1 < x0)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = dy-2*dx;            /* Initial value of d */
	int incrNE = 2 * (dy - dx);             /* Increment used for move to NE */
	int incrN = 2 * -dx;     /* Increment used for move to N */
	int x = x0;
	int y = y0;
	SETPIXEL(x, y, intensity);        /* The start pixel */

	while (y <= y1) {
		if (d < 0) {                /* Choose NE */
			d += incrNE;
			y++;
			x++;
		}
		else {                    /* Choose E */
			d += incrN;
			y++;
		}
		SETPIXEL(x, y, intensity);    /* The selected pixel closest to the line */
	} /* while */
}

//TODO: This is slope > 1
//void DrawLine(int x0, int y0, int x1, int y1, Color intensity)
//{
//	if (x1 < x0)
//	{
//		std::swap(x0, x1);
//		std::swap(y0, y1);
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	int d = dy - 2 * dx;            /* Initial value of d */
//	int incrNE = 2 * (dy - dx);             /* Increment used for move to NE */
//	int incrN = 2 * -dx;     /* Increment used for move to N */
//	int x = x0;
//	int y = y0;
//	SETPIXEL(x, y, intensity);        /* The start pixel */
//
//	while (y <= y1) {
//		if (d < 0) {                /* Choose NE */
//			d += incrNE;
//			y++;
//			x++;
//		}
//		else {                    /* Choose E */
//			d += incrN;
//			y++;
//		}
//		SETPIXEL(x, y, intensity);    /* The selected pixel closest to the line */
//	} /* while */
//}
//
//TODO: This is 0 < slope < 1
//void DrawLine(int x0, int y0, int x1, int y1, Color intensity)
//{
//	if (x1 < x0)
//	{
//		std::swap(x0, x1);
//		std::swap(y0, y1);
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	int d = 2 * dy - dx;            /* Initial value of d */
//	int incrE = 2 * dy;             /* Increment used for move to E */
//	int incrNE = 2 * (dy - dx);     /* Increment used for move to NE */
//	int x = x0;
//	int y = y0;
//	SETPIXEL(x, y, intensity);        /* The start pixel */
//
//	while (x < x1) {
//		if (d <= 0) {                /* Choose E */
//			d += incrE;
//			x++;
//		}
//		else {                    /* Choose NE */
//			d += incrNE;
//			x++;
//			y++;
//		}
//		SETPIXEL(x, y, intensity);    /* The selected pixel closest to the line */
//	} /* while */
//}
//
//TODO: This is -1 < slope < 0
//void DrawLine(int x0, int y0, int x1, int y1, Color intensity)
//{
//	if (x1 < x0)
//	{
//		std::swap(x0, x1);
//		std::swap(y0, y1);
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	int d = 2 * dy + dx;            /* Initial value of d */
//	int incrE = 2 * dy;             /* Increment used for move to E */
//	int incrSE = 2 * (dy + dx);     /* Increment used for move to NE */
//	int x = x0;
//	int y = y0;
//	SETPIXEL(x, y, intensity);        /* The start pixel */
//
//	while (x < x1) {
//		if (d >= 0) {                /* Choose E */
//			d += incrE;
//			x++;
//		}
//		else {                    /* Choose NE */
//			d += incrSE;
//			x++;
//			y--;
//		}
//		SETPIXEL(x, y, intensity);    /* The selected pixel closest to the line */
//	} /* while */
//}
//
//TODO: This is slope < -1
//void DrawLine(int x0, int y0, int x1, int y1, Color intensity)
//{
//	if (x1 < x0)
//	{
//		std::swap(x0, x1);
//		std::swap(y0, y1);
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	int d = -dy;            /* Initial value of d */
//	int incrS = 2 * dx;             /* Increment used for move to E */
//	int incrSE = 2 * (dy + dx);     /* Increment used for move to NE */
//	int x = x0 - 1;
//	int y = y0 + 1;
//	SETPIXEL(x, y, intensity);        /* The start pixel */
//
//	while (y >= y1 + 1) {
//		if (d < 0) {                /* Choose E */
//			d += incrS;
//			y--;
//		}
//		else {                    /* Choose NE */
//			d += incrSE;
//			x++;
//			y--;
//		}
//		SETPIXEL(x, y, intensity);    /* The selected pixel closest to the line */
//	} /* while */
//}