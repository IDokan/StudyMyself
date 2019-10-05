/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Driver.cpp
Purpose: source file for bit vector assignment
Project: sinil.gang / CS225 / Vector
Author: Kang Shinil
Creation date: 10/05/2019
- End Header ----------------------------------------------------------------
*/
#include <iostream>		// std::cout, std::cerr
#include "Vector.h"			// vector

void test1( ) {
	try
	{
		Vector<int> vec;

		for(int i = 1; i <= 20; i++) {
			vec.PushBack(i);
			std::cout << "Current Size: " << vec.Count() << "   Capacity: " << vec.Capacity() << std::endl;
		}
		std::cout << vec << std::endl;
	}
	catch (const char* string)
	{
		std::cerr << string << std::endl;
	}
}

void test2( ) {
	try
	{
		Vector<int> vec;
		for(int i = 1; i <= 20; i++) {
			vec.PushBack(i);
		}
		std::cout << vec[0] << "  " << vec[4] << "  " << vec[9] << "  " << vec[14] << "  " << vec[19] << std::endl;
	}
	catch (const char* string)
	{
		std::cerr << string << std::endl;
	}
}

void test3( ) {
	try
	{
		Vector<int> vec;

		for(int i = 1; i <= 20; i++) {
			vec.PushBack(i);
		}
		std::cout << *vec.Find(5) << "  " << *vec.Find(15) << std::endl;
	}
	catch (const char* string)
	{
		std::cerr << string << std::endl;
	}
}

void test4( ) {
	try
	{
		Vector<int> vec;

		for(int i = 1; i <= 20; i++) {
			vec.PushBack(i);
		}
		vec.Remove(vec.Find(5));
		vec.Remove(vec.Find(15));
		vec.Remove(vec.Find(12));
		vec.Remove(vec.Find(19));
		vec.Remove(vec.Find(7));
		vec.Remove(vec.Find(2));

		std::cout << vec << std::endl;
	}
	catch (const char* string)
	{
		std::cerr << string << std::endl;
	}
}

void test5( ) {
	try
	{
		Vector<int> vec;

		for(int i = 1; i <= 10; i++) {
			vec.PushBack(i);
		}
		vec.Insert(vec.Find(5), 11);
		vec.Insert(vec.Find(2), 12);
		vec.Insert(vec.Find(7), 13);
		vec.Insert(vec.Find(9), 14);
		vec.Insert(vec.Find(1), 15);
		vec.Insert(vec.Find(5), 16);

		std::cout << vec << std::endl;
	}
	catch (const char* string)
	{
		std::cerr << string << std::endl;
	}
}

void test6( ) {
	try
	{
		Vector<char> charVec;
		for (char i = 'a'; i <= 'z'; i++) {
			charVec.PushBack(i);
		}
		std::cout << charVec << std::endl;

		Vector<double> doubleVec;
		for (int i = 1; i <= 10; i++) {
			doubleVec.PushBack(i / 10.0);
		}
		std::cout << doubleVec << std::endl;
	}
	catch (const char* string)
	{
		std::cerr << string << std::endl;
	}
}

int main(int argc, char **argv) {
	int testNum = -1;
	if(argc > 1) {
		testNum = atoi(argv[1]);
	}

	if(testNum == -1) {
		test1( );
		test2( );
		test3( );
		test4( );
		test5( );
		test6( );
	} else {
		typedef void(*Test)(void);
		Test Tests[] = {
			test1, test2, test3, test4, test5, test6
		};

		Tests[testNum-1]( );
	}
}