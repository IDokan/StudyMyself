/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: Final Assignment
Project: Exploring C++
Author: sinil.gang
Creation date: 12.05.2019
- End Header ----------------------------------------------------------------
*/

#include <iostream>
#include <array>
#include <map>
#include <mutex>


struct vector2
{
	float x, y;
};

/*	1. auto multiple initializer
 *	auto [a, b, ...] = obj;
 *	The type of obj must be Destructurable:
 *	
 *	Must be public
 *	Must be direct members of the type or members of the same public base class of the type
 *	Cannot be anonymous unions
 *
 *	uses regular auto deduction rules(auto const, auto&, auto&&)
 */
void autoMultipleInitializerDemo()
{
	vector2 tmp{ 10.f, 5.f };
	auto [u, v] = tmp;
	std::cout << u << std::endl;
	std::cout << v << std::endl;
	
	std::array<int, 3> arr = { 1, 2, 3 };
	auto const [x, y, z] = arr;
	auto& [q, w, e] = arr;
	auto&& [i, o, p] = arr;
	
	// std::tuple, std::pair
	std::map<int, std::string> map;
	for (auto&& [key, value] : map)
	{
		std::cout << "Key : " << key << std::endl;
		std::cout << "Value : " << value << std::endl << std::endl;
	}
}

bool GetData(int* return_storage)
{
	if (return_storage == nullptr)
	{
		return false;
	}
	
	*return_storage = 30;
	return true;
}

struct my_string
{
	char PopBack();
	char PopBackC17();

private:
	std::mutex mtx_;
	std::string pool_;
};

char my_string::PopBack()
{
	char s = '\0';

	{	std::lock_guard<std::mutex> l(mtx_);
		if (!pool_.empty())
		{
			std::swap(s, pool_.back());
			pool_.pop_back();
		}
	}

	// No need to lock here
	std::cout << "Pop character is " << s << std::endl;

	// pool_ is still locked
	return s;
}

char my_string::PopBackC17()
{
	char s = '\0';

	
	if (std::lock_guard<std::mutex> l(mtx_);
		!pool_.empty())
	{
		std::swap(s, pool_.back());
		pool_.pop_back();
	}

	std::cout << "Pop character is " << s << std::endl;

	return s;
}

/* 2. selection sequence with initializers */
void selectionSequenceWithInitializersDemo()
{
	int storage;
	bool is_success = GetData(&storage);
	if (	is_success == true)
	{
		std::cout << "Success! result is " << storage << std::endl;
	}

	// Scope
	// Nothing alive
	if (bool x = false;
		x == true)
	{
		// x is alive
	}
	else if(bool y = false;
		y == true)
	{
		// Both x and y are alive
	}
	else
	{
		// Both x and y are still alive
	}
	// Nothing alive

	// Become useful when dealing with locks
	// TODO: Go to string_pool::pop()
	

	// It also worked in switch statement
}

/*	3. if constexpr
 *
 *	Compile-time conditional statements.
 *	The condition must be a <constexpr> expression.
 *	Statements are discarded if their branch is not taken.
 *		Discarded statements can use variables that are declared
 *			but not defined.
 *		Discarded statements in templates are not instantiated.
 */

/* Before Code */
template <typename T0>
void print(T0&& t0)
{
	std::cout << std::forward<T0>(t0) << std::endl;
}
template <typename T0, typename... Ts>
void print(T0&& t0, Ts&& ... ts)
{
	print(std::forward<T0>(t0));
	print(std::forward<Ts>(ts)...);
}
/* End of Before Code */

/* Buggy Code*/
template<typename T0, typename... Ts>
void printBuggy(T0&& t0, Ts&& ... ts)
{
	std::cout << std::forward<T0>(t0) << std::endl;

	// Difference with only if is instantiated or not
	if (sizeof...(ts))
	{
		printBuggy(std::forward<Ts>(ts)...);
	}
}

/* C++17 Code*/
template<typename T0, typename... Ts>
void printC17(T0&& t0, Ts&& ... ts)
{
	std::cout << std::forward<T0>(t0) << std::endl;

	// Difference with only if is instantiated or not
	if constexpr (sizeof...(ts))
	{
		printC17(std::forward<Ts>(ts)...);
	}
}

void IfConstexprDemo()
{
	print(1, 2, 3, 4);
	//printBuggy(1, 2, 3, 4);
	printC17(1, 2, 3, 4);
}

void PrintUsage(const char* program_name)
{
	std::cout << "usage : " << program_name << " <test case>" << "\n\n";
	std::cout << "0 -> autoMultipleInitializerDemo()\n";
	std::cout << "1 -> selectionSequenceWithInitializersDemo()\n";
	std::cout << "2 -> IfConstexprDemo()\n";
}

/* End of C++17 Code*/
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		PrintUsage(argv[0]);
		return 0;
	}
	int test_number = atoi(argv[1]);
	if (test_number < 0 || test_number > 2)
	{
		PrintUsage(argv[0]);
		return 0;
	}

	switch (test_number)
	{
	case 0:
		autoMultipleInitializerDemo();
		break;
	case 1:
		selectionSequenceWithInitializersDemo();
		break;
	case 2:
		IfConstexprDemo();
		break;
	default:
		break;
	}
}
