/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: cipher.cpp
Purpose: Final
Project: Bit Cipher
Author: sinil.gang
Creation date: 12.22.2019
- End Header ----------------------------------------------------------------
*/

#include <iostream>
#include <map>				// std::map
#include <vector>			// In order to inherite it
#include <algorithm>		// std::for_each
#include "cipher.h"

std::string data = { " etaoinshrdlcumwfgypbvkjxqz" };

int numBits(int value);
int GetNumGroups();

class BitStream : public std::vector<char>
{
public:
	static const int sizeOfChar = 8;
private:
	struct BitsData;
public:
	BitStream()
	{
		clear();
	}
	BitStream(std::vector<char> compressed)
		: vector(std::move(compressed))
	{
	}
	BitsData operator[](const size_t index) noexcept
	{
		return BitsData(*this, index);
	}

	int UpdateDictionary() noexcept
	{
		int result = 0;
		::data.clear();

		for (size_t i = 0; i < size(); ++i)
		{
			// Update result and data
			const char c = (at(i));
			result += sizeOfChar;
			::data.push_back(c);


			if (IsValidChar(c) == false)
			{
				// Return immediately
				return result;
			}
		}

		return 0;
	}

	void MakeDictionary(const std::string& str) noexcept
	{
		std::map<char, size_t>dictionaryToBeSorted{};

		InitializeDictionaryGivenString(dictionaryToBeSorted, str);

		const std::vector<std::pair<size_t, char>> sortedDictionary = SortDictionary(dictionaryToBeSorted);

		UpdateStreamAndDataGivenDictionary(sortedDictionary);
	}

private:
	static size_t GetOffset(size_t position)
	{
		return (7 - (position % sizeOfChar));
	}

	bool IsValidChar(char c) const noexcept
	{
		return (c != '\0');
	}

	void UpdateStreamAndDataGivenDictionary(const std::vector<std::pair<size_t, char>>& dictionary)
	{
		push_back(' ');
		// store in stream
		std::for_each(std::begin(dictionary), std::end(dictionary), [&](const std::pair<size_t, char>& element)
			{
				push_back(element.second);
			}
		);
		push_back('\0');


		// Update Data
		::data.clear();
		const size_t dictionarySize = size();
		for (size_t i = 0; i < dictionarySize; ++i)
		{
			::data.push_back(at(i));
		}
	}

	void InitializeDictionaryGivenString(std::map<char, size_t>& dictionary, const std::string& str)
	{
		dictionary.clear();

		// In each string
		std::for_each(std::begin(str), std::end(str), [&](const char& c)
			{
				if (c != ' ')
				{
					dictionary[c]++;
				}
			}
		);
	}

	std::vector<std::pair<size_t, char>> SortDictionary(std::map<char, size_t>& dictionary)
	{
		std::vector<std::pair<size_t, char>> sortedData;

		sortedData.reserve(dictionary.size());
		for (auto& it : dictionary)
		{
			sortedData.emplace_back(it.second, it.first);
		}

		std::stable_sort(sortedData.begin(), sortedData.end(), [&](const std::pair<size_t, char>& lhs, const std::pair<size_t, char>& rhs)
			{
				return lhs.first > rhs.first;
			}
		);

		return sortedData;
	}

private:
	// Proxy struct
	struct BitsData
	{
	private:
		void IncreaseSizeIfNeeded(size_t currentCharSize) const
		{
			while (currentCharSize >= stream.size())
			{
				stream.push_back(0);
			}
		}
	public:
		BitStream& stream;
		size_t byte_pos;
		size_t bit_pos;

		BitsData(BitStream& bitStream, size_t index)
			:stream(bitStream), byte_pos(index / sizeOfChar), bit_pos(index) {}

		// Invoked when proxy is used to modify the value.
		BitsData& operator = (const bool& rhs) noexcept
		{
			IncreaseSizeIfNeeded(byte_pos);

			stream.at(byte_pos) |= rhs << GetOffset(bit_pos);

			return *this;
		}

		bool EndOfData() const noexcept
		{
			return (byte_pos) >= (stream.size());
		}

		operator unsigned int() const
		{
			if (EndOfData() == true)
			{
				return 0;
			}

			return static_cast<bool>(stream.at(byte_pos) & (1 << GetOffset(bit_pos)));
		}

	};

private:
};

int ReadBits(BitStream& stream, int& index, int numBits) {
	int value = 0;
	for (int i = numBits - 1; i >= 0; --i) {
		value |= (stream[index++]) << i;
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

bool GetGroupAndIndex(const BitStream& stream, char letter, int& groupIndex, int& charIndex) {
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
	int index = stream.UpdateDictionary();
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
	stream.MakeDictionary(uncompressed);
	int groupIndex, charIndex;
	int bitsPerGroup = numBits(GetNumGroups() - 1);
	int index = stream.size() * BitStream::sizeOfChar;


	for (char c : uncompressed) {
		if (GetGroupAndIndex(stream, c, groupIndex, charIndex) == true) {
			SetBits(stream, index, groupIndex, bitsPerGroup);
			SetBits(stream, index, charIndex, groupIndex + 1); // charIndex uses groupIndex + 1 bits to store it
		}
	}
	return stream;
}