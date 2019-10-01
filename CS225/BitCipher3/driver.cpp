/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: driver.cpp
Purpose: This is the driver file to be used for the bit cipher assignment
Project: CS225 BitCipher
Author: Kevin Wright
Creation date: 8/20/2019
- End Header ----------------------------------------------------------------
*/

#include <iostream>
#include <string>
#include <vector>
#include "cipher.h"

void printBits(std::vector<char> data) {
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
}

void runTest( std::string originalText )
{
	std::vector<char> compressedText;
	std::string uncompressedText;
	std::cout << "Original string = "; 
	std::cout << originalText << std::endl;

	compressedText = encode(originalText);

	std::cout << "Compressed string = ";
    printBits(compressedText);
	std::cout << std::endl;

	uncompressedText = decode(compressedText);
	uncompressedText.erase(uncompressedText.find_last_not_of(' ')+1);	// Trim the spaces
	std::cout << "Uncompressed string = " << uncompressedText << std::endl;

	if (originalText == uncompressedText) {
		std::cout << "Strings Match" << std::endl;
	}
	else {
		std::cout << "Strings Do NOT Match" << std::endl;
	}
}

void test0() { runTest( "nopqrstu" ); }
void test1() { runTest( "cc" ); }
void test2() { runTest( "ccc"); }
void test3() { runTest( "cccc" ); }
void test4() { runTest( "hello" ); }
void test5() { runTest( "xaxa" ); }
void test6() { runTest( "abcdefghijklmnopqrstuvwxyz" ); }
void test7() { runTest( "kdjasfhkdslfhksdjhfkldsfhlskdfjh" ); }
void test8() { runTest( "aardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvarkaardvark" ); }


int main (int argc, char ** argv) {
	int test_num = -1;

	typedef void(*Test)(void);
	Test Tests[] = {
		test0,	test1, test2, test3, test4, test5, test6, test7, test8
	};
	int numTests = static_cast<int>(sizeof(Tests) / sizeof(*Tests));
	if (argc > 1) {
		test_num = std::atoi(argv[1]);
	}

	if (test_num >= 0 && test_num < numTests) {
		Tests[test_num]();
	}
	else {
		for (int i = 0; i < numTests; i++) {
			Tests[i]();
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif
	return 0;
}
