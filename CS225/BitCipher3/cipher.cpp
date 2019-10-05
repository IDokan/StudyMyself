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
#include <cmath>	// pow
#include "cipher.h"


namespace
{
	const int NUMGROUPS = 4;
	std::string groups[NUMGROUPS] = {
		" e",	// 2	/0b1
		"taoi",	// 4	/0b11
		"nshrdlcu",	// 8	/ 0b111
		"mwfgypbvkjxqz"	// 13	/ 0b1111
	};

	std::string data = { " etaoinshrdlcumwfgypbvkjxqz" };
}

// return index matched group bit
// For example,
// when 0 come, 0 out.
// when 1 come, 2 out.
// when 2 come, 6 out.
// when 3 come, 14 out.
// when 4 come, 30 out.
int GetGroupIndexOffset(int groupBit)
{
	int offSet = 0;
	for (int i = 0; i < groupBit; ++i)
	{
		offSet += 2<<i;
	}
	return offSet;
}

char TryGetHuffmanChar(int groupBit, int indexBit)
{
	const unsigned int stringIndex = indexBit + GetGroupIndexOffset(groupBit);
	if (stringIndex >= data.length())
	{
		throw "IndexBit is out of range!";
	}
	return data.at(stringIndex);
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

// If value is a hover value, make pull it to ground.
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
	size_t charLocation = data.find(character);
	
	if (charLocation != std::string::npos)
	{
		int countGroup = 0;
		// How can I make 12 into 2 and 6
		while(GetGroupIndexOffset(countGroup + 1) <= int(charLocation))
		{
			++countGroup;
		}
		charLocation -= GetGroupIndexOffset(countGroup);
		
		sizeOfCharacter = char(countGroup + 0x03);
		return char((countGroup << char(countGroup+1)) | char(charLocation));
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

		// Fill the container in normally
		if (bitPosition > 0)
		{
			container = char(container | tmpCharacter << (bitPosition));
		}
		// if it is full or over than capacity, then push_back in vector<char>
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
	constexpr char groupBitMask = 0x03;		// 0b11, group bit is always 2bit
	const int numOfGroupBit = numBits(groupBitMask);
	char bitPosition = initialBitPosition;
	char indexBitMask;

	size_t iterator = 0;
	char element = compressed.at(iterator);
	const size_t sizeOfVector = compressed.size();

	while (iterator != sizeOfVector)
	{
		char groupBit;
		// First of all, since group bit is always 2 bit, substituted by Num of Group Bit.
		bitPosition = char(bitPosition - numOfGroupBit);
		// If substitution groupBit is -1, I've read every bit in current byte and I should read more 1 bit.
		if (bitPosition == -1)
		{
			// I will read left one bit in current bit
			groupBit = char(GetBinaryValue(element, 0x01) << 1);
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			bitPosition = char(bitPosition + initialBitPosition);
			// And Get a First bit in next bit
			groupBit = char(groupBit | GetBinaryValue(element, char(0x80)));
		}
		// If substitution groupBit is -2, I've read every bit in current byte and I should read more 2 bit.
		else if (bitPosition == -2)
		{
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			bitPosition = char(bitPosition + initialBitPosition);
			groupBit = GetBinaryValue(element, char(groupBitMask << bitPosition));
		}
		else
		{
			groupBit = GetBinaryValue(element, char(groupBitMask << bitPosition));
		}

		// Since indexBitMask always has 2^(groupBit+1) - 1, I make it formula
		indexBitMask = char(std::pow(2, groupBit + 1) - 1);

		const char numOfIndexMask = char(numBits(indexBitMask));
		bitPosition = char(bitPosition - numOfIndexMask);

		char indexBit = 0;
		// If bit position has no enough space or fulfilled, push back and prepare to next byte
		if (bitPosition < 0)
		{
			indexBit = GetBinaryValue(char(element << abs(bitPosition)), indexBitMask);
			if (++iterator == sizeOfVector)
			{
				return str;
			}
			element = compressed.at(iterator);
			bitPosition = char(bitPosition + initialBitPosition);
			indexBit = char(indexBit | GetBinaryValue(element, indexBitMask = char(indexBitMask << bitPosition)));
		}
		// Otherwise, continue to store.
		else
		{
			indexBitMask = char(indexBitMask << bitPosition);
			indexBit = GetBinaryValue(element, indexBitMask);
		}
		char c = 0;
		try
		{
			c = TryGetHuffmanChar(groupBit, indexBit);
		}
		catch (const char* exception)
		{
			return str;
		}
		str += c;
	}

	return str;
}
