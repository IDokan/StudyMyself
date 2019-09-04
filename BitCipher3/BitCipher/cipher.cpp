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


// Helper function for DEBUG
void printBit(std::vector<char> data) {
	for (char c : data) {
		for (int bitIndex = 7; bitIndex >= 0; --bitIndex) {
			if (c & (1 << bitIndex)) {
				std::cout << "1";
			}
			else {
				std::cout << "0";
			}
			if (bitIndex % 4 == 0) {
				std::cout << '\'';
			}
		}
	}
	std::cout << std::endl;
}


namespace
{
	static const int NUMGROUPS = 4;
	static std::string groups[NUMGROUPS] = {
		" e",	// 2	/0b01
		"taoi",	// 4	/0b11
		"nshrdlcu",	// 7	/ 0b111
		"mwfgypdvkjxqz"	// 13	/ 0b1111
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
	while(value >>= 1)
	{
		++requiredBits;
	}
	return requiredBits;
}

// If value is a hover value, make push it to ground.
int GetCountToFloorValue(int value) noexcept
{
	int count = 0;
	while (value % 2 == 0)
	{
		value >>= 1;
		++count;
	}
	return count;
}

char GetBinaryValue(char bits, char mask) noexcept
{
	int count = GetCountToFloorValue(mask);
	return (bits & mask) >> count;
}

// Return decoded value given character.
char GetEncodedChar(char character, int& sizeOfChatacter) noexcept
{
	for (int countGroup = 0; countGroup < NUMGROUPS; ++countGroup)
	{
		const int charLocation = groups[countGroup].find(character);
		if(charLocation != std::string::npos)
		{
			switch (countGroup)
			{
			case 0:
				sizeOfChatacter = 3;
				break;
			case 1:
				sizeOfChatacter = 4;
				break;
			case 2:
				sizeOfChatacter = 5;
				break;
			case 3:
				sizeOfChatacter = 6;
				break;
			default:
				sizeOfChatacter = 0;
				break;
			}
			return (countGroup << (countGroup + 1)) | charLocation;
		}
	}
	return '\0';
}

std::vector<char> encode(std::string uncompressed)
{
	std::vector<char> compressed;
	
	constexpr char initialBitPosition = sizeof(char)*8;
	int bitPosition = initialBitPosition;

	char container = 0;
	for (const auto & element : uncompressed)
	{
		int numCharacter;

		char tmpCharacter = GetEncodedChar(element, numCharacter);
		bitPosition -= numCharacter;


		if (bitPosition > 0)
		{
			container |= tmpCharacter << (bitPosition );
		}
		else if (bitPosition < 0)
		{
			// under implementing
			container |= tmpCharacter >> (abs(bitPosition ));
			compressed.push_back(container);
			container = tmpCharacter << (8 - abs(bitPosition ));	// wrong code
			bitPosition += 8;

			//Test
			printBit(compressed);
		}
		else if (bitPosition == 0)
		{
			container |= tmpCharacter;
			compressed.push_back(container);
			bitPosition = initialBitPosition;
			container = 0;

			//Test
			printBit(compressed);
		}
	}
	compressed.push_back(container);
	return compressed;
}

// Now Implementing
std::string decode(std::vector<char> compressed)
{
	return std::string("test");
	std::string str;
	str.clear();

	char mask;
	for (const auto & element : compressed)
	{
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
		catch(const char* exception)
		{
			std::cout << exception;
		}
		str += c;
	}

	return str;
}
