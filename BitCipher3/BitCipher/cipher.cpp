/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: cipher.cpp
Purpose: source file for bit cipher assignment
Project: sinil.gang / CS225 / Bit Cipher
Author: Kang Shinil
Creation date: 09/02/2019
- End Header ----------------------------------------------------------------
*/

#include <vector>
#include <string>
#include "cipher.h"

namespace
{
	static const int NUMGROUPS = 4;
	static std::string groups[NUMGROUPS] = {
		" e",
		"taoi",
		"nshrdlcu",
		"mwfgypdvkjxqz"
	};
}

int numBits(int value)
{
	int requiredBits = 1;
	while(value >>= 1)
	{
		++requiredBits;
	}
	return requiredBits;
}

char GetBinaryValue(char bits, char mask)
{
	return (bits & mask) >> numBits(mask);
}

// Return decoded value given character.
char GetEncodedChar(char character)
{
	for (int countGroup = 0; countGroup < NUMGROUPS; ++countGroup)
	{
		const int charLocation = groups[countGroup].find(character);
		if(charLocation != std::string::npos)
		{
			return (countGroup << numBits(charLocation)) | charLocation;
		}
	}
	return '\0';
}

std::vector<char> encode(std::string uncompressed)
{
	std::vector<char> compressed;
	for (const auto & element : uncompressed)
	{
			compressed.push_back(GetEncodedChar(element));
	}
	return compressed;
}

// Now Implementing
std::string decode(std::vector<char> compressed)
{
	std::string str;
	char mask;

	for (const auto & element : compressed)
	{
		int result = GetBinaryValue(element, 0xA0);
		switch (result)
		{
		case 0:
			mask = 0x20;
			break;
		case 1:
			mask = 0x30;
			break;
		case 2:
			mask = 0x38;
			break;
		case 3:
			mask = 0x3C;
			break;
		default:
			break;
		}
	}

	return str;
}
