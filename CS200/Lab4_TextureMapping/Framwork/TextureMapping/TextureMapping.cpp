/*
1. sinil.gang
2. CS200 Lab 4 - Texture Mapping
3. CS200
4. Fall 2019
*/

#include <CS200/Image.hpp>
#include <cstdlib>
#include <algorithm>
#include "CS200/RenderOption.hpp"

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

const int WIDTH = 512;
const int HEIGHT = 512;

Image gImage{ WIDTH, HEIGHT, BEAUTY_BUSH_PINK };

struct Vector3D
{
	Vector3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	static float dot(const Vector3D& vec1, const Vector3D& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	static Vector3D cross(const Vector3D& vec1, const Vector3D& vec2)
	{
		return Vector3D(vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x);
	}

	float x;
	float y;
	float z;
};

struct Vertex2D
{
	Vertex2D(float x, float y, const Color& color, float u, float v)
		: x(x), y(y), color(color), u(u), v(v) {}

	Vector3D operator-(const Vertex2D& vertex) const
	{
		return Vector3D(x - vertex.x, y - vertex.y, 0);
	}

	int operator*(const Vector3D& vector) const
	{
		return static_cast<int>(x * vector.x + y * vector.y);
	}

	float x = 0;
	float y = 0;
	Color color;
	float u = 0;
	float v = 0;
};

struct Point3D
{
	Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
	Point3D(const Vertex2D& v) : x(v.x), y(v.y), z(0) {}

	Vector3D operator-(const Point3D& rhs) const
	{
		return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	
	int operator*(const Vector3D& v) const
	{
		return static_cast<int>(x * v.x + y * v.y);
	}

	float x;
	float y;
	float z;
};

struct TextureCoordinate
{
	TextureCoordinate(float u = 0.f, float v = 0.f):u(u), v(v) {}
	
	float u;
	float v;
};

int ROUND(float f) { return static_cast<int>(floor(f + 0.5f)); }

void SETPIXEL(int column, int row, Color intensity) { gImage.SetPixel(column, row, intensity); }

void DrawTriangle(const Vertex2D& v1, const Vertex2D& v2, const Vertex2D& v3, const RenderOption & option);

int main(void)
{
	//Vertices
	Vertex2D v1(0, 0, RED, 0, 0); // top left
	Vertex2D v2(WIDTH, HEIGHT, BLUE, 3, 3); // bottom right
	Vertex2D v3(WIDTH, 0, GREEN, 3, 0); // top right
	Vertex2D v4(0, HEIGHT, BLACK, 0, 3); // bottom left

	//Render option
	Texture texture("awesomeface.png");
	RenderOption option;
	option.texture = &texture;
	option.texture_func = TextureFunction::REPEAT;
	option.cc_mode = ColorCombineMode::DECAL;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("repeat_decal.png");

	option.texture_func = TextureFunction::REPEAT;
	option.cc_mode = ColorCombineMode::MODULATE;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("repeat_modulate.png");

	option.texture_func = TextureFunction::REPEAT;
	option.cc_mode = ColorCombineMode::REPLACE;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("repeat_replace.png");

	option.texture_func = TextureFunction::MIRROR;
	option.cc_mode = ColorCombineMode::DECAL;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("mirror_decal.png");

	option.texture_func = TextureFunction::MIRROR;
	option.cc_mode = ColorCombineMode::MODULATE;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("mirror_modulate.png");

	option.texture_func = TextureFunction::MIRROR;
	option.cc_mode = ColorCombineMode::REPLACE;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("mirror_replace.png");

	option.texture_func = TextureFunction::CLAMP;
	option.cc_mode = ColorCombineMode::DECAL;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("clamp_decal.png");

	option.texture_func = TextureFunction::CLAMP;
	option.cc_mode = ColorCombineMode::MODULATE;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("clamp_modulate.png");

	option.texture_func = TextureFunction::CLAMP;
	option.cc_mode = ColorCombineMode::REPLACE;
	DrawTriangle(v1, v2, v3, option);
	DrawTriangle(v1, v4, v2, option);
	gImage.SaveToPNG("clamp_replace.png");

	return 0;
}



/**
 * \param decisions value which used for calculation (Array of decisions)
 * \return bool value whether they are inside or not
 */
bool AreTheyAllInside(int* decisions)
{
	return (decisions[0] >= 0) &&
		(decisions[1] >= 0) &&
		(decisions[2] >= 0);
}

template<typename T>
void InitialArray(T* arr, T* arr2, const int size)
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

enum class AttributeType {
	RED,
	GREEN,
	BLUE,
	U_TEXTURECOORDINATE,
	V_TEXTURECOORDINATE,
	NUMOFATTRIBUTES,
};

Vector3D CalculateAttributeVector(const Vertex2D v1, const Vertex2D& v2, AttributeType attributeType)
{
	Vector3D vector{v2.x - v1.x, v2.y-v1.y, 0};
	switch (attributeType)
	{
	case AttributeType::RED:
		vector.z =  static_cast<float>(v2.color.Red - v1.color.Red);
		break;
	case AttributeType::GREEN:
		vector.z =  static_cast<float>(v2.color.Green - v1.color.Green);
		break;
	case AttributeType::BLUE:
		vector.z =  static_cast<float>(v2.color.Blue - v1.color.Blue);
		break;
	case AttributeType::U_TEXTURECOORDINATE:
		vector.z = v2.u - v1.u;
		break;
	case AttributeType::V_TEXTURECOORDINATE:
		vector.z = v2.v - v1.v;
		break;
	default:
		vector.z = -1;
		break;
	}
	return vector;
}

float GetAttributeStartingDecision(const Vector3D& normalVector, const Point3D& startingPoint, const Vertex2D& anyPoint, AttributeType attributeType)
{
	switch (attributeType) {
	case AttributeType::RED: return -(normalVector.x * startingPoint.x + normalVector.y * startingPoint.y - (normalVector.x * anyPoint.x + normalVector.y * anyPoint.y + normalVector.z * anyPoint.color.Red)) / normalVector.z;
	case AttributeType::GREEN: return -(normalVector.x * startingPoint.x + normalVector.y * startingPoint.y - (normalVector.x * anyPoint.x + normalVector.y * anyPoint.y + normalVector.z * anyPoint.color.Green)) / normalVector.z;
	case AttributeType::BLUE: return -(normalVector.x * startingPoint.x + normalVector.y * startingPoint.y - (normalVector.x * anyPoint.x + normalVector.y * anyPoint.y + normalVector.z * anyPoint.color.Blue)) / normalVector.z;
	case AttributeType::U_TEXTURECOORDINATE: return -(normalVector.x * startingPoint.x + normalVector.y * startingPoint.y - (normalVector.x * anyPoint.x + normalVector.y * anyPoint.y + normalVector.z * anyPoint.u)) / normalVector.z;
	case AttributeType::V_TEXTURECOORDINATE: return -(normalVector.x * startingPoint.x + normalVector.y * startingPoint.y - (normalVector.x * anyPoint.x + normalVector.y * anyPoint.y + normalVector.z * anyPoint.v)) / normalVector.z;
	case AttributeType::NUMOFATTRIBUTES: break;
	default:;
	}
	return -1.0;
}

float clamp(float x, float max = 255.f, float min = 0.f)
{
	if (x >= max)
	{
		return max;
	}
	else if (x <= min)
	{
		return min;
	}
	return (x);
}

float mirror(float num)
{
	int intNum = static_cast<int>(num);
	if (intNum % 2 == 0)	// Even
	{
		return num - floor(num);
	}
	else							  // Odd
	{
		return 1.f - (num - floor(num));
	}
}

Color GetBlendedColorWithOption(const Color& surfaceColor, TextureCoordinate&& textureCoordinate, const RenderOption& option)
{
	switch (option.texture_func) {
		case TextureFunction::REPEAT:
		default:
			textureCoordinate.u -= floor(textureCoordinate.u);
			textureCoordinate.v -= floor(textureCoordinate.v);
		break;
		case TextureFunction::MIRROR:
			textureCoordinate.u = mirror(textureCoordinate.u);
			textureCoordinate.v = mirror(textureCoordinate.v);
		break;
		case TextureFunction::CLAMP:
			textureCoordinate.u = static_cast<float> (clamp(textureCoordinate.u, 1.f, 0.f));
			textureCoordinate.v = static_cast<float> (clamp(textureCoordinate.v, 1.f, 0.f));
		break;
	}

	int x = static_cast<int>(textureCoordinate.u * (option.texture->GetTextureWidth() - 1));
	int y = static_cast<int>(textureCoordinate.v * (option.texture->GetTextureHeight() - 1));
	const Color textureColor = option.texture->GetTextureColor(x, y);

	Color FinalColor{};
	switch (option.cc_mode) {
		case ColorCombineMode::DECAL:
			FinalColor.Red = surfaceColor.Red + textureColor.Alpha * (textureColor.Red - surfaceColor.Red);
			FinalColor.Green = surfaceColor.Green + textureColor.Alpha * (textureColor.Green - surfaceColor.Green);
			FinalColor.Blue = surfaceColor.Blue+ textureColor.Alpha * (textureColor.Blue - surfaceColor.Blue);
			break;
		case ColorCombineMode::MODULATE:
			FinalColor = surfaceColor * textureColor;
		break;
		case ColorCombineMode::REPLACE:
			FinalColor = textureColor;
			break;
		default: ;
	}
	return FinalColor;
}

void DrawTriangle(const Vertex2D& v1, const Vertex2D& v2, const Vertex2D& v3, const RenderOption& option)
{
	const Point3D smallPoint3D = GetSmallPoint3D(v1, v2, v3);
	const Point3D largePoint3D = GetLargePoint3D(v1, v2, v3);

	// Given Lines
	const Vector3D line1{ v1 - v3 };
	const Vector3D line2{ v3 - v2 };
	const Vector3D line3{ v2 - v1 };

	// Normal Vector
	const Vector3D nLine1{ line1.y, -(line1.x) };
	const Vector3D nLine2{ line2.y, -(line2.x) };
	const Vector3D nLine3{ line3.y, -(line3.x) };

	const int size = 3;
	int startingDecision[size]
	{
		smallPoint3D * nLine1 - v3 * nLine1,
		smallPoint3D * nLine2 - v2 * nLine2,
		smallPoint3D * nLine3 - v1 * nLine3
	};


	// Initialize color vectors
	const Vector3D redVector1 = CalculateAttributeVector(v2, v1, AttributeType::RED);
	const Vector3D redVector2 = CalculateAttributeVector(v3, v1, AttributeType::RED);
	const Vector3D greenVector1 = CalculateAttributeVector(v2, v1, AttributeType::GREEN);
	const Vector3D greenVector2 = CalculateAttributeVector(v3, v1, AttributeType::GREEN);
	const Vector3D blueVector1 = CalculateAttributeVector(v2, v1, AttributeType::BLUE);
	const Vector3D blueVector2 = CalculateAttributeVector(v3, v1, AttributeType::BLUE);

	const Vector3D redNormalVector = Vector3D::cross(redVector1, redVector2);
	const Vector3D greenNormalVector = Vector3D::cross(greenVector1, greenVector2);
	const Vector3D blueNormalVector = Vector3D::cross(blueVector1, blueVector2);

	float startingColorDecision[size]
	{
		GetAttributeStartingDecision(redNormalVector, smallPoint3D, v1, AttributeType::RED),
		GetAttributeStartingDecision(greenNormalVector, smallPoint3D, v1, AttributeType::GREEN),
		GetAttributeStartingDecision(blueNormalVector, smallPoint3D, v1, AttributeType::BLUE),
	};

	float colorXOffsets[size]
	{
		(-redNormalVector.x) / redNormalVector.z,
		(-greenNormalVector.x) / greenNormalVector.z,
		(-blueNormalVector.x) / blueNormalVector.z,
	};

	float colorYOffsets[size]
	{
		(-redNormalVector.y) / redNormalVector.z,
		(-greenNormalVector.y) / greenNormalVector.z,
		(-blueNormalVector.y) / blueNormalVector.z,
	};

	// Initialize texture coordinate vectors
	const Vector3D uVector1 = CalculateAttributeVector(v2, v1, AttributeType::U_TEXTURECOORDINATE);
	const Vector3D uVector2 = CalculateAttributeVector(v3, v1, AttributeType::U_TEXTURECOORDINATE);
	const Vector3D vVector1 = CalculateAttributeVector(v2, v1, AttributeType::V_TEXTURECOORDINATE);
	const Vector3D vVector2 = CalculateAttributeVector(v3, v1, AttributeType::V_TEXTURECOORDINATE);

	const Vector3D uNormalVector = Vector3D::cross(uVector1, uVector2);
	const Vector3D vNormalVector = Vector3D::cross(vVector1, vVector2);

	const int textureCoordinateSize = 2;
	float startingTextureCoordinateDecision[textureCoordinateSize]
	{
		GetAttributeStartingDecision(uNormalVector, smallPoint3D, v1, AttributeType::U_TEXTURECOORDINATE),
		GetAttributeStartingDecision(vNormalVector, smallPoint3D, v1, AttributeType::V_TEXTURECOORDINATE),
	};

	float textureCoordinateXOffset[textureCoordinateSize]
	{
		(-uNormalVector.x) / uNormalVector.z,
		(-vNormalVector.x) / vNormalVector.z,
	};

	float textureCoordinateYOffset[textureCoordinateSize]
	{
		(-uNormalVector.y) / uNormalVector.z,
		(-vNormalVector.y) / vNormalVector.z,
	};

	int tempDecision[size]{ startingDecision[0], startingDecision[1], startingDecision[2] };
	float tempColorDecision[size]{ startingColorDecision[0], startingColorDecision[1], startingColorDecision[2] };
	float tempTextureCoordinateDecision[textureCoordinateSize]{ startingTextureCoordinateDecision[0], startingTextureCoordinateDecision[1] };

	// Casting value of starting and destination point.
	const int smallRowPointInteger = static_cast<int>(smallPoint3D.y);
	const int smallColumnPointInteger = static_cast<int>(smallPoint3D.x);
	const int largeRowPointInteger = static_cast<int>(largePoint3D.y);
	const int largeColumnPointInteger = static_cast<int>(largePoint3D.x);
	
	for (int row = smallRowPointInteger; row < largeRowPointInteger; ++row)
	{
		// Initialize tempDecision as initial value of starting decision.
		InitialArray<int>(tempDecision, startingDecision, size);
		InitialArray<float>(tempColorDecision, startingColorDecision, size);
		InitialArray<float>(tempTextureCoordinateDecision, startingTextureCoordinateDecision, textureCoordinateSize);

		for (int column = smallColumnPointInteger; column < largeColumnPointInteger; ++column)
		{
			if (AreTheyAllInside(tempDecision))
			{
				SETPIXEL(column, row, GetBlendedColorWithOption(
					Color(
						static_cast<unsigned char>(clamp(tempColorDecision[0])),
						static_cast<unsigned char>(clamp(tempColorDecision[1])),
						static_cast<unsigned char>(clamp(tempColorDecision[2]))),
					TextureCoordinate(
						tempTextureCoordinateDecision[0],
						tempTextureCoordinateDecision[1]),
					option
				));
			}
			// Add X value
			tempDecision[0] += static_cast<int>(nLine1.x);
			tempDecision[1] += static_cast<int>(nLine2.x);
			tempDecision[2] += static_cast<int>(nLine3.x);
			// Add X color offset
			tempColorDecision[0] += colorXOffsets[0];
			tempColorDecision[1] += colorXOffsets[1];
			tempColorDecision[2] += colorXOffsets[2];
			// Add U offset
			tempTextureCoordinateDecision[0] += textureCoordinateXOffset[0];
			tempTextureCoordinateDecision[1] += textureCoordinateXOffset[1];
		}
		// Add Y value
		startingDecision[0] += static_cast<int>(nLine1.y);
		startingDecision[1] += static_cast<int>(nLine2.y);
		startingDecision[2] += static_cast<int>(nLine3.y);
		// Add Y color offset
		startingColorDecision[0] += colorYOffsets[0];
		startingColorDecision[1] += colorYOffsets[1];
		startingColorDecision[2] += colorYOffsets[2];
		// Add V offset
		startingTextureCoordinateDecision[0] += textureCoordinateYOffset[0];
		startingTextureCoordinateDecision[1] += textureCoordinateYOffset[1];
	}
}

