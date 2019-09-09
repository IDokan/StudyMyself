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
		" e",	// 2	/0b1
		"taoi",	// 4	/0b11
		"nshrdlcu",	// 7	/ 0b111
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
char GetCountToFloorValue(char value) noexcept
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
	int count = GetCountToFloorValue(mask);
	return (bits & mask) >> count;
}

// Return decoded value given character.
char GetEncodedCharAndSize(char character, char& sizeOfCharacter) noexcept
{
	for (int countGroup = 0; countGroup < NUMGROUPS; ++countGroup)
	{
		const size_t charLocation = groups[countGroup].find(character);
		if (charLocation != std::string::npos)
		{
			switch (countGroup)
			{
			case 0:
				sizeOfCharacter = 3;
				break;
			case 1:
				sizeOfCharacter = 4;
				break;
			case 2:
				sizeOfCharacter = 5;
				break;
			case 3:
				sizeOfCharacter = 6;
				break;
			default:
				sizeOfCharacter = 0;
				break;
			}
			return (countGroup << (countGroup + 1)) | char(charLocation);
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
		bitPosition -= sizeOfCharacter;

		// Fill the container and if it is full, push_back in vector<char>
		if (bitPosition > 0)
		{
			container |= tmpCharacter << (bitPosition);
		}
		else if (bitPosition < 0)
		{
			container |= tmpCharacter >> (abs(bitPosition));
			compressed.push_back(container);
			container = tmpCharacter << (initialBitPosition - abs(bitPosition));	// wrong code
			bitPosition += initialBitPosition;
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
	constexpr char groupBitMask = 0b11;
	char groupBitPosition = 8;
	char mask;

	// What should be a condition statement?
	char iterator = 0;
	char element = compressed.at(iterator);
	const size_t sizeOfVector = compressed.size();
	
	while (iterator != sizeOfVector)
	{
		char result;
		groupBitPosition -= 2;
		if (groupBitPosition == -1)
		{
			result = GetBinaryValue(element, 0b1) << 1;
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			groupBitPosition += initialBitPosition;
			result |= GetBinaryValue(element, 0x80);
		}
		else if (groupBitPosition == -2)
		{
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			groupBitPosition += initialBitPosition;
			result = GetBinaryValue(element, char(groupBitMask << groupBitPosition));
		}
		else
		{
			result = GetBinaryValue(element, char(groupBitMask << groupBitPosition));
		}

		// Is there any better solutions?
		switch (result)
		{
		case 0b00:
			mask = 0b1;
			break;
		case 0b01:
			mask = 0b11;
			break;
		case 0b10:
			mask = 0b111;
			break;
		case 0b11:
			mask = 0b1111;
			break;
		default:
			mask = 0x00;
			break;
		}

		const char numOfMask = numBits(mask);
		groupBitPosition -= numOfMask;

		char index = 0;
		if (groupBitPosition < 0)
		{
			index = GetBinaryValue(element << abs(groupBitPosition), mask);
			if (++iterator == int(sizeOfVector))
			{
				return str;
			}
			element = compressed.at(iterator);
			groupBitPosition += initialBitPosition;
			index |= GetBinaryValue(element, mask <<= groupBitPosition);
		}
		else
		{
			mask <<= groupBitPosition;
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
