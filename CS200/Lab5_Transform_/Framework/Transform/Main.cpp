/*
 * Jooho Jeong
 * Lab 5 framework
 * CS200
 * Fall 2019
 */
#include <CS200/Image.hpp>
#include <cstdlib>
#include "CS200/Triangle.h"
#include "Transform.h"
#include "CS200/Object.h"
#include "CS200/Line.h"

using namespace CS200;

const Color BLACK{ 0x000000FF };
const Color WHITE{ 0xFFFFFFFF };
const Color JAGUAR_BLACK{ 0x1F0322FF };
const Color DISCO_PURPLE{ 0x8a1c7cFF };
const Color CABARET_RED{ 0xda4167FF };
const Color BATTLESHIP_GRAY{ 0x899d78FF };
const Color BEAUTY_BUSH_PINK{ 0xf0bcd4FF };

const Color RED(255, 0, 0);
const Color GREEN(0, 255, 0);
const Color BLUE(0, 0, 255);

const Matrix I(Vector(1, 0), Vector(0, 1), Point());

const int WIDTH = 512;
const int HEIGHT = 512;

Image gImage{ WIDTH, HEIGHT, WHITE };

int main(void)
{
	/////////////////////////////////
	//Faces
	Texture red("face_red.png");
	Object red_face(red, Point(-150, 50));
	red_face.Rotate(30);
	red_face.SetScale(Vector(0.1f, 0.1f));

	Texture green("face_green.png");
	Object green_face(green, Point(100, 100));
	green_face.SetScale(Vector(0.2f, 0.2f));
	green_face.Rotate(-40);

	Texture blue("face_blue.png");
	Object blue_face(blue, Point(-30, -130));
	blue_face.SetScale(Vector(0.3f, 0.3f));
	blue_face.Rotate(220);

	/////////////////////////////////
	//test0 - world frame
	Matrix viewport_transform(Vector(1, 0), Vector(0, -1), Point(WIDTH / 2, HEIGHT / 2));
	red_face.Draw(gImage, I, viewport_transform);
	green_face.Draw(gImage, I, viewport_transform);
	blue_face.Draw(gImage, I, viewport_transform);

	int HALF_WIDTH = WIDTH / 2;
	int HALF_HEIGHT = HEIGHT / 2;
	DrawLine(0, HALF_HEIGHT, WIDTH, HALF_HEIGHT, BLACK, gImage);
	DrawLine(HALF_WIDTH, 0, HALF_WIDTH, HEIGHT, BLACK, gImage);

	gImage.SaveToPNG("test0_world.png");
	gImage.Clear(WHITE);

	/////////////////////////////////
	//test1 - red's view
	Matrix world_to_relative = red_face.WorldToRelative();
	red_face.Draw(gImage, world_to_relative, viewport_transform);
	green_face.Draw(gImage, world_to_relative, viewport_transform);
	blue_face.Draw(gImage, world_to_relative, viewport_transform);
	DrawLine(0, HALF_HEIGHT, WIDTH, HALF_HEIGHT, RED, gImage);
	DrawLine(HALF_WIDTH, 0, HALF_WIDTH, HEIGHT, RED, gImage);

	gImage.SaveToPNG("test1_red's_view.png");
	gImage.Clear(WHITE);

	/////////////////////////////////
	//test2 - green's view
	world_to_relative = green_face.WorldToRelative();
	red_face.Draw(gImage, world_to_relative, viewport_transform);
	green_face.Draw(gImage, world_to_relative, viewport_transform);
	blue_face.Draw(gImage, world_to_relative, viewport_transform);
	DrawLine(0, HALF_HEIGHT, WIDTH, HALF_HEIGHT, GREEN, gImage);
	DrawLine(HALF_WIDTH, 0, HALF_WIDTH, HEIGHT, GREEN, gImage);

	gImage.SaveToPNG("test2_green's_view.png");
	gImage.Clear(WHITE);

	/////////////////////////////////
	//test3 - blue's view
	world_to_relative = blue_face.WorldToRelative();
	red_face.Draw(gImage, world_to_relative, viewport_transform);
	green_face.Draw(gImage, world_to_relative, viewport_transform);
	blue_face.Draw(gImage, world_to_relative, viewport_transform);
	DrawLine(0, HALF_HEIGHT, WIDTH, HALF_HEIGHT, BLUE, gImage);
	DrawLine(HALF_WIDTH, 0, HALF_WIDTH, HEIGHT, BLUE, gImage);

	gImage.SaveToPNG("test3_blue's_view.png");
	gImage.Clear(WHITE);

	/////////////////////////////////
	//test4 - world -> relative -> world
	world_to_relative = blue_face.WorldToRelative();
	Matrix relative_to_world = blue_face.RelativeToWorld();
	Matrix world_to_world = relative_to_world * world_to_relative;

	red_face.Draw(gImage, world_to_world, viewport_transform);
	green_face.Draw(gImage, world_to_world, viewport_transform);
	blue_face.Draw(gImage, world_to_world, viewport_transform);
	DrawLine(0, HALF_HEIGHT, WIDTH, HALF_HEIGHT, BLACK, gImage);
	DrawLine(HALF_WIDTH, 0, HALF_WIDTH, HEIGHT, BLACK, gImage);

	gImage.SaveToPNG("test4_world_to_world.png");
	gImage.Clear(WHITE);

	return 0;
}

