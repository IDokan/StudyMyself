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

#include <vector>	// std::vector
#include <string>	// std::string
#include "cipher.h"
#include <iostream> // std::cout

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

char TryGetHuffmanChar(int groupBit, int indexBit)
{
	if (groupBit < 0 || groupBit >= NUMGROUPS)
	{
		throw "GroupBit is out of range!";
	}
	if (indexBit < 0 || indexBit >= int(groups[groupBit].length()))
	{
		throw "IndexBit is out of range!";
	}
	return groups[groupBit].at(indexBit);
}

// If value is a hover value, make push it to ground.
int GetCountToFloorValue(int value)
{
	int count = 0;
	while (value % 2 == 0)
	{
		value >>= 1;
		++count;
	}
	return count;
}

char GetBinaryValue(char bits, char mask)
{
	int count = GetCountToFloorValue(mask);
	return (bits & mask) >> count;
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
	str.clear();

	char mask;
	for (const auto & element : compressed)
	{
		//int result = GetBinaryValue(element, 0xA0);
		//switch (result)
		//{
		//case 0:
		//	mask = 0x20;
		//	break;
		//case 1:
		//	mask = 0x30;
		//	break;
		//case 2:
		//	mask = 0x38;
		//	break;
		//case 3:
		//	mask = 0x3C;
		//	break;
		//default:
		//	break;
		//}

		char result = GetBinaryValue(element, 0x18);
		switch (result)
		{
		case 0b10:
			mask = 0x07;
			break;
		default:
			mask = 0x00;
			break;
		}
		char c = '\0';
		int index = GetBinaryValue(element, mask);
		try
		{
			c = TryGetHuffmanChar(result, index);
		}
		catch (const std::string& str)
		{
			std::cout << str;
		}
		catch(const char* str)
		{
			std::cout << str;
		}
		str += c;
	}

	return str;
}
