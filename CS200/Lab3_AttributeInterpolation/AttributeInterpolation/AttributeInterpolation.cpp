#include <CS200/Image.hpp>
#include <cstdlib>

using namespace CS200;

const Color BLACK{ 0x000000FF };
const Color WHITE{ 0xFFFFFFFF };
const Color JAGUAR_BLACK{ 0x1F0322FF };
const Color DISCO_PURPLE{ 0x8a1c7cFF };
const Color CABARET_RED{ 0xda4167FF };
const Color BATTLESHIP_GRAY{ 0x899d78FF };

const Color RED(255, 0, 0);
const Color GREEN(0, 255, 0);
const Color BLUE(0, 0, 255);

const int WIDTH = 256;
const int HEIGHT = 256;
const Color BEAUTY_BUSH_PINK{ 0xf0bcd4FF };
Image gImage{ WIDTH, HEIGHT, BEAUTY_BUSH_PINK };

struct Vector3D
{
	Vector3D(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    static int dot(const Vector3D& vec1, const Vector3D& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    static Vector3D cross(const Vector3D& vec1, const Vector3D& vec2)
    {
        return Vector3D(vec1.y * vec2.z - vec1.z * vec2.y,
            vec1.z * vec2.x - vec1.x * vec2.z,
            vec1.x * vec2.y - vec1.y * vec2.x);
    }

    int x;
    int y;
    int z;
};

struct Vertex2D
{
    Vertex2D(int x, int y, const Color & color)
        : x(x), y(y), color(color) {}

	Vector3D operator-(const Vertex2D & v) const
    {
        return Vector3D(x - v.x, y - v.y, 0);
    }

	int operator*(const Vector3D& v) const
	{
		return x * v.x + y * v.y;
	}
	
    int x = 0;
    int y = 0;
    Color color;
};

struct Point3D
{
    Point3D(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
    Point3D(const Vertex2D & v) : x(v.x), y(v.y), z(0) {}

	Vector3D operator-(const Point3D & rhs) const
    {
        return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
    }
	
	int operator*(const Vector3D& v) const
	{
		return x * v.x + y * v.y;
	}

    int x;
    int y;
    int z;
};

int ROUND(double f) { return static_cast<int>(floor(f + 0.5f)); }

void SETPIXEL(int column, int row, Color intensity) { gImage.SetPixel(column, row, intensity); }

void DrawTriangle(const Vertex2D & v1, const Vertex2D & v2, const Vertex2D & v3);

int main(void)
{
    Vertex2D v1(128, 10, RED);
    Vertex2D v2(246, 246, GREEN);
    Vertex2D v3(10, 246, BLUE);
    DrawTriangle(v1, v2, v3);

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
	for (int i = 0; i < size; ++i)
	{
		arr[i] = arr2[i];
	}
}

Point3D GetSmallPoint3D(const Point3D& p1, const Point3D& p2, const Point3D& p3)
{
	Point3D result;
	result.x = (p1.x < p2.x) ? p1.x : p2.x;
	result.x = (result.x < p3.x) ? result.x : p3.x;
	result.y = (p1.y < p2.y) ? p1.y : p2.y;
	result.y = (result.y < p3.y) ? result.y : p3.y;

	return result;
}

Point3D GetLargePoint3D(const Point3D& p1, const Point3D& p2, const Point3D& p3)
{
	Point3D result;
	result.x = (p1.x > p2.x) ? p1.x : p2.x;
	result.x = (result.x > p3.x) ? result.x : p3.x;
	result.y = (p1.y > p2.y) ? p1.y : p2.y;
	result.y = (result.y > p3.y) ? result.y : p3.y;

	return result;
}

enum class ColorType {
	RED,
	GREEN,
	BLUE,
	NUMOFCOLORS,
};

Vector3D CalculateColorVector(const Vertex2D v1, const Vertex2D& v2, ColorType colorType)
{
	switch (colorType)
	{
	case ColorType::RED:
		return Vector3D{ v2.x - v1.x, v2.y - v1.y, v2.color.Red - v1.color.Red };
		break;
	case ColorType::GREEN:
		return Vector3D{ v2.x - v1.x, v2.y - v1.y, v2.color.Green - v1.color.Green };
		break;
	case ColorType::BLUE:
		return Vector3D{ v2.x - v1.x, v2.y - v1.y, v2.color.Blue - v1.color.Blue };
		break;
	case ColorType::NUMOFCOLORS: break;
	default: ;
	}
	
}

void DrawTriangle(const Vertex2D & v1, const Vertex2D & v2, const Vertex2D & v3)
{
	const Point3D smallPoint3D = GetSmallPoint3D(v1, v2, v3);
	const Point3D largePoint3D = GetLargePoint3D(v1, v2, v3);

	// Given Lines
	const Vector3D line1{ v3 - v1 };
	const Vector3D line2{ v2 - v3 };
	const Vector3D line3{ v1 - v2 };

	// Normal Vector
	const Vector3D nLine1{ line1.y, -line1.x };
	const Vector3D nLine2{ line2.y, -line2.x };
	const Vector3D nLine3{ line3.y, -line3.x };

	const Vector3D redVector1 = CalculateColorVector(v2, v1, ColorType::RED);
	const Vector3D redVector2 = CalculateColorVector(v3, v1, ColorType::RED);
	const Vector3D greenVector1 = CalculateColorVector(v2, v1, ColorType::GREEN);
	const Vector3D greenVector2 = CalculateColorVector(v3, v1, ColorType::GREEN);
	const Vector3D blueVector1 = CalculateColorVector(v2, v1, ColorType::BLUE);
	const Vector3D blueVector2 = CalculateColorVector(v3, v1, ColorType::BLUE);

	const int size = 3;
	int startingDecision[size]
	{
		smallPoint3D * nLine1 - v3 * nLine1,
		smallPoint3D * nLine2 - v2 * nLine2,
		smallPoint3D * nLine3 - v1 * nLine3
	};
	int tempDecision[size]{ startingDecision[0], startingDecision[1], startingDecision[2] };

	for (int row = smallPoint3D.y; row < largePoint3D.y; ++row)
	{
		// Initialize tempDecision as initial value of starting decision.
		InitialArray(tempDecision, startingDecision, size);

		for (int column = smallPoint3D.x; column < largePoint3D.x; ++column)
		{
			if (AreTheyAllInside(tempDecision))
			{
				
				SETPIXEL(column, row, BLACK);
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

