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
 *	Must be direct members of the type or members of the same public base clas of the type
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
	auto [x, y, z] = arr;
	std::cout << x << std::endl;
	std::cout << y << std::endl;
	std::cout << z << std::endl;

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
	char s;

	std::lock_guard<std::mutex> l(mtx_);
	if (!pool_.empty())
	{
		std::swap(s, pool_.back());
		pool_.pop_back();
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

/*	2. selection sequence with initializers
 *
 * 
 */
void selectionSequenceWithInitializersDemo()
{
	int storage;
	if (bool is_success = GetData(&storage);
		is_success == true)
	{
		std::cout << "Success! result is " << storage;
	}
	if (bool is_success = GetData(nullptr);
		is_success == true)
	{
		std::cout << "Success! result is " << storage;
	}

	// Scope
	// Nothing alive
	if (bool x = false;
		x)
	{
		// X is alive
	}
	else if(bool y = false;
		y)
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

/* C++17 Code*/
template<typename T0, typename... Ts>
void printC17(T0&& t0, Ts&&... ts)
{
	std::cout << std::forward<T0>(t0) << std::endl;

	// Difference with only if is instanciated or not
	/*	Lecturer's comment!
	 *	if this condition is not true,
	 *	statement needs to parse, but not instantiated.
	 *
	 *	When you're on the last element here,
	 *		but there's no function overload
	 *		that matches over here is fine
	 *		because this was never instantiated.
	 *
	 *	Now if you didn't have the if constexpr here,
	 *		you would be trying to call print
	 *		with zero arguments
	 *		when this parameter pack is empty,
	 *		so you get a compiler error.
	 *	But with if constexpr,
	 *		you don't get the compiler error,
	 *		you just never instantiate.
	 */
	if constexpr(sizeof...(ts))
	{
		printC17(std::forward<Ts>(ts)...);
	}
}

/*	Fold Expression
 *
 * 
 */

/* End of C++17 Code*/
int main(int argc, char* argv[])
{
	printC17(1, 2, 3, 4);
}
