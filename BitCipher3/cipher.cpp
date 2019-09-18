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
#include <cmath>	// pow


namespace
{
	const int NUMGROUPS = 4;
	std::string groups[NUMGROUPS] = {
		" e",	// 2	/0b1
		"taoi",	// 4	/0b11
		"nshrdlcu",	// 8	/ 0b111
		"mwfgypbvkjxqz"	// 13	/ 0b1111
	};
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

int numBits(int value) noexcept
{
	int requiredBits = 1;
	while (value >>= 1)
	{
		++requiredBits;
	}
	return requiredBits;
}

// If value is a hover value, make push it to ground.
char GetCountToFloorValue(int value) noexcept
{
	char count = 0;
	while (value % 2 == 0)
	{
		value >>= 1;
		++count;
	}
	return count;
}

char GetBinaryValue(const unsigned char bits, const unsigned char mask) noexcept
{
	const char count = GetCountToFloorValue(mask);
	return char((bits & mask) >> count);
}

// Return decoded value given character.
char GetEncodedCharAndSize(char character, char& sizeOfCharacter) noexcept
{
	for (unsigned char countGroup = 0; countGroup < NUMGROUPS; ++countGroup)
	{
		const size_t charLocation = groups[countGroup].find(character);
		if (charLocation != std::string::npos)
		{
			sizeOfCharacter = char(countGroup + 0x03);
			return char((countGroup << char(countGroup + 1)) | char(charLocation));
		}
	}
	return '\0';
}

std::vector<char> encode(std::string uncompressed)
{
	std::vector<char> compressed;

	constexpr char initialBitPosition = 8;
	char bitPosition = initialBitPosition;

	char container = 0;
	for (const auto& element : uncompressed)
	{
		char sizeOfCharacter;

		const char tmpCharacter = GetEncodedCharAndSize(element, sizeOfCharacter);
		bitPosition = char(bitPosition - sizeOfCharacter);

		// Fill the container and if it is full, push_back in vector<char>
		if (bitPosition > 0)
		{
			container = char(container | tmpCharacter << (bitPosition));
		}
		else if (bitPosition < 0)
		{
			container = char(container | tmpCharacter >> (abs(bitPosition)));
			compressed.push_back(container);
			container = char(tmpCharacter << (initialBitPosition - abs(bitPosition)));
			bitPosition = char(bitPosition + initialBitPosition);
		}
		else if (bitPosition == 0)
		{
			container |= tmpCharacter;
			compressed.push_back(container);
			bitPosition = initialBitPosition;
			container = 0;
		}
	}
	compressed.push_back(container);
	return compressed;
}


std::string decode(std::vector<char> compressed)
{
	std::string str;
	str.clear();

	constexpr char initialBitPosition = 8;
	constexpr char groupBitMask = 0x03;
	char groupBitPosition = initialBitPosition;
	char mask;

	size_t iterator = 0;
	char element = compressed.at(iterator);
	const size_t sizeOfVector = compressed.size();

	while (iterator != sizeOfVector)
	{
		char result;
		groupBitPosition = char(groupBitPosition - 2);
		if (groupBitPosition == -1)
		{
			result = char(GetBinaryValue(element, 0x01) << 1);
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			groupBitPosition = char(groupBitPosition + initialBitPosition);
			result = char(result | GetBinaryValue(element, char(0x80)));
		}
		else if (groupBitPosition == -2)
		{
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			groupBitPosition = char(groupBitPosition + initialBitPosition);
			result = GetBinaryValue(element, char(groupBitMask << groupBitPosition));
		}
		else
		{
			result = GetBinaryValue(element, char(groupBitMask << groupBitPosition));
		}

		mask = char(std::pow(2, result + 1) - 1);

		const char numOfMask = char(numBits(mask));
		groupBitPosition = char(groupBitPosition - numOfMask);

		char index = 0;
		if (groupBitPosition < 0)
		{
			index = GetBinaryValue(char(element << abs(groupBitPosition)), mask);
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			groupBitPosition = char(groupBitPosition + initialBitPosition);
			index = char(index | GetBinaryValue(element, mask = char(mask << groupBitPosition)));
		}
		else
		{
			mask = char(mask << groupBitPosition);
			index = GetBinaryValue(element, mask);
		}
		char c = 0;
		try
		{
			c = TryGetHuffmanChar(result, index);
		}
		catch (const char* exception)
		{
			std::cout << exception;
		}
		str += c;
	}

	return str;
}
