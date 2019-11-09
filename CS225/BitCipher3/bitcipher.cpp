/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: bitcipher.cpp
Purpose: Assignment 4 Bit Cipher with Proxy
Project: Bit Cipher
Author: sinil.gang
Creation date: 11.09.2019
- End Header ----------------------------------------------------------------
*/

#include <iostream>
#include <vector>
#include "cipher.h"

std::string data = { " etaoinshrdlcumwfgypbvkjxqz" };

int numBits(int value);
int GetNumGroups();

class BitStream {
private:
	struct BitsData;
	const int sizeOfChar = 8;
public:
	BitStream()
	{
		bits.clear();
	}
	BitStream(const std::vector<char>& compressed)
		: BitStream()
	{
		// The loop to convert from vector<char> to vector<bool>
		const size_t sizeOfCompressed = compressed.size();
		for (size_t compressedCount = 0; compressedCount < sizeOfCompressed; ++compressedCount)
		{
			char c = compressed.at(compressedCount);
			StoreCharacter(compressed.at(compressedCount));
		}
	}
	BitsData operator[](const size_t index) noexcept
	{
		return BitsData(*this, index);
	}
	operator const std::vector<char>() const
	{
		// The loop to convert from vector<bool> to vector<char>
		std::vector<char> result;

		const size_t sizeOfBits = bits.size();
		for (size_t bitsCount = 0; bitsCount < sizeOfBits; bitsCount += 8)
		{
			result.push_back(LoadCharacter(bitsCount, sizeOfBits));
		}

		return result;
	}

private:
	// Helper functions
	void StoreCharacter(char character)
	{
		for (int bitCount = sizeOfChar - 1; bitCount >= 0; --bitCount)
		{
			bits.push_back(character & (1 << bitCount));
		}
	}
	char LoadCharacter(int index, int totalSize) const
	{
		char resultContainer = 0;
		for (int count = sizeOfChar - 1; count >= 0 && index < totalSize; --count)
		{
			resultContainer |= bits.at(index++) << count;
		}
		return resultContainer;
	}
	
	// Proxy struct
	struct BitsData
	{
		BitStream& stream;
		size_t pos;

		BitsData(BitStream& bitStream, size_t index)
			:stream(bitStream), pos(index) {}

		// Invoked when proxy is used to modify the value.
		void operator = (const bool& rhs)
		{
			// If bits is too small to store value, make it large.
			while (pos >= stream.bits.size())
			{
				stream.bits.push_back(false);
			}
			// Assign data
			stream.bits.at(pos) = rhs;
		}


		bool EndOfData()
		{
			// If bits has no more than (current position + the number of group bits) elements, left data of bits are invalid data.
			// The reason why is at least, bits should have 1 + the number of group bits.
			return (pos+ numBits(GetNumGroups() - 1)) >= stream.bits.size();
		}

		char operator<<(int i)
		{
			return stream.bits.at(pos) << i;
		}

	};

	std::vector<bool> bits;
};

int ReadBits(BitStream& stream, int& index, int numBits) {
	int value = 0;
	for (int i = numBits - 1; i >= 0; --i) {
		value |= stream[index++] << i;
	}
	return value;
}
void SetBits(BitStream& stream, int& index, int value, int numBits) {
	for (int i = numBits - 1; i >= 0; --i) {
		stream[index++] = static_cast<bool>(value & (1 << i));
	}
}

// Counts the number of bits needed to store the given value 
int numBits(int value) {
	int requiredBits = 1;
	while (value >>= 1) {
		requiredBits++;
	}
	return requiredBits;
}

int GetNumGroups() {
	int groupCount = 0;
	int size = data.size();
	while (size > 1 << (groupCount + 1)) {
		groupCount++;
		size -= 1 << (groupCount + 1);
	}
	return groupCount + 1;
}

bool GetGroupAndIndex(char letter, int& groupIndex, int& charIndex) {
	charIndex = data.find(letter);
	if (charIndex == std::string::npos) {
		return false;
	}
	groupIndex = 0;
	while (charIndex >= 1 << (groupIndex + 1)) {
		charIndex -= 1 << (groupIndex + 1);
		groupIndex++;
	}
	return true;
}

char GetLetter(int groupIndex, int charIndex) {
	int charIndexFromGroup = 0;
	while (groupIndex > 0) {
		charIndexFromGroup += 1 << groupIndex;
		groupIndex--;
	}
	return data[charIndexFromGroup + charIndex];
}


std::string decode(std::vector<char> compressed) {
	BitStream stream(compressed);
	int index = 0;
	std::string uncompressed;
	int bitsPerGroup = numBits(GetNumGroups() - 1);

	while (stream[index].EndOfData() == false) {
		int groupIndex = ReadBits(stream, index, bitsPerGroup);
		int charIndex = ReadBits(stream, index, groupIndex + 1);	// charIndex uses groupIndex + 1 bits to store it
		uncompressed += GetLetter(groupIndex, charIndex);
	}
	return uncompressed;
}

std::vector<char> encode(std::string uncompressed) {
	BitStream stream;
	int index = 0;
	int groupIndex, charIndex;
	int bitsPerGroup = numBits(GetNumGroups() - 1);

	for (char c : uncompressed) {
		if (GetGroupAndIndex(c, groupIndex, charIndex) == true) {
			SetBits(stream, index, groupIndex, bitsPerGroup);
			SetBits(stream, index, charIndex, groupIndex + 1); // charIndex uses groupIndex + 1 bits to store it
		}
	}
	return stream;
}