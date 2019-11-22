/*
 * Jooho Jeong
 * Graphics Library
 * CS200
 * Fall 2019
 */
#include <CS200/Image.hpp>
#include <cstdlib>
#include "CS200/Circle.hpp"
#include "CS200/Rectangle.h"
#include "CS200/Intersection.h"

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

void test0()
{
	Point p(220, 256);
	Color p_color = BLACK;

	Rectangle rect(Point(100, 300), Point(200, 250), GREY);
	if (PointVSAABB(p, rect))
	{
		rect.SetColor(DISCO_PURPLE);
		p_color = Color(0, 255, 0, 255);
	}
	rect.Draw(gImage);
	gImage.SetPixel(p.x, p.y, p_color);
	gImage.SaveToPNG("test0.png");
	gImage.Clear(WHITE);
}

void test1()
{
	Point p(120, 276);
	Color p_color = BLACK;

	Rectangle rect(Point(100, 300), Point(200, 250), GREY);
	if (PointVSAABB(p, rect))
	{
		rect.SetColor(DISCO_PURPLE);
		p_color = Color(0, 255, 0, 255);
	}
	rect.Draw(gImage);
	gImage.SetPixel(p.x, p.y, p_color);
	gImage.SaveToPNG("test1.png");
	gImage.Clear(WHITE);
}

void test2()
{
	Point p(220, 256);
	Color p_color = BLACK;

	Circle circle(256, 256, 100, GREY);
	if (PointVSCircle(p, circle))
	{
		circle.SetColor(DISCO_PURPLE);
		p_color = Color(0, 255, 0, 255);
	}
	circle.Draw(gImage);
	gImage.SetPixel(p.x, p.y, p_color);
	gImage.SaveToPNG("test2.png");
	gImage.Clear(WHITE);
}

void test3()
{
	Point p(220, 256);
	Color p_color = BLACK;

	Circle circle(256, 256, 10, GREY);
	if (PointVSCircle(p, circle))
	{
		circle.SetColor(DISCO_PURPLE);
		p_color = Color(0, 255, 0, 255);
	}
	circle.Draw(gImage);
	gImage.SetPixel(p.x, p.y, p_color);
	gImage.SaveToPNG("test3.png");
	gImage.Clear(WHITE);
}

void test4()
{
	Circle circle1(256, 256, 150, GREY);
	Circle circle2(100, 100, 75, GREY);
	if (CircleVSCircle(circle1, circle2))
	{
		circle1.SetColor(DISCO_PURPLE);
		circle2.SetColor(DISCO_PURPLE);
	}
	circle1.Draw(gImage);
	circle2.Draw(gImage);
	gImage.SaveToPNG("test4.png");
	gImage.Clear(WHITE);
}

void test5()
{
	Circle circle1(256, 256, 140, GREY);
	Circle circle2(100, 100, 75, GREY);
	if (CircleVSCircle(circle1, circle2))
	{
		circle1.SetColor(DISCO_PURPLE);
		circle2.SetColor(DISCO_PURPLE);
	}
	circle1.Draw(gImage);
	circle2.Draw(gImage);
	gImage.SaveToPNG("test5.png");
	gImage.Clear(WHITE);
}

void test6()
{
	Rectangle rect1(Point(100, 200), Point(200, 100), GREY);
	Rectangle rect2(Point(250, 350), Point(400, 200), GREY);
	if (AABBVSAABB(rect1, rect2))
	{
		rect1.SetColor(DISCO_PURPLE);
		rect2.SetColor(DISCO_PURPLE);
	}
	rect1.Draw(gImage);
	rect2.Draw(gImage);
	gImage.SaveToPNG("test6.png");
	gImage.Clear(WHITE);
}

void test7()
{
	Rectangle rect1(Point(200, 400), Point(300, 300), GREY);
	Rectangle rect2(Point(250, 350), Point(400, 200), GREY);
	if (AABBVSAABB(rect1, rect2))
	{
		rect1.SetColor(DISCO_PURPLE);
		rect2.SetColor(DISCO_PURPLE);
	}
	rect1.Draw(gImage);
	rect2.Draw(gImage);
	gImage.SaveToPNG("test7.png");
	gImage.Clear(WHITE);
}

void test8()
{
	Rectangle rect(Point(200, 400), Point(300, 300), GREY);
	Circle circle(256, 256, 30, GREY);
	if (CircleVSAABB(circle, rect))
	{
		rect.SetColor(DISCO_PURPLE);
		circle.SetColor(DISCO_PURPLE);
	}
	rect.Draw(gImage);
	circle.Draw(gImage);
	gImage.SaveToPNG("test8.png");
	gImage.Clear(WHITE);
}

void test9()
{
	Rectangle rect(Point(250, 400), Point(350, 300), GREY);
	Circle circle(180, 240, 100, GREY);
	if (CircleVSAABB(circle, rect))
	{
		rect.SetColor(DISCO_PURPLE);
		circle.SetColor(DISCO_PURPLE);
	}
	rect.Draw(gImage);
	circle.Draw(gImage);
	gImage.SaveToPNG("test9.png");
	gImage.Clear(WHITE);
}


int main(void)
{
	//point vs AABB (1)
	test0();

	//point vs AABB (2)
	test1();

	//point vs circle (1)
	test2();

	//point vs circle (2)
	test3();

	//circle vs circle (1)
	test4();

	//circle vs circle (2)
	test5();

	//AABB vs AABB (1)
	test6();

	//AABB vs AABB (2)
	test7();

	//circle vs AABB (1)
	test8();

	//circle vs AABB (2)
	test9();

    return 0;
}

