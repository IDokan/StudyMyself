/******************************************************************************
filename    CPPThread2.cpp
author      Jookyung Lee, Sinil Gang
DP email    jookyung.lee@digipen.edu, sinil.gang@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using CPP Threads.

******************************************************************************/

#include <iostream>
#include <thread>

namespace
{
	const int COUNT = 100000;
	int g_global = 0;
}

void Increment()
{
	for(int i = 0; i < COUNT; ++i)
	{
		++g_global;
	}
	return;
}

int main(void)
{
	std::thread t1{Increment};

	for(int i = 0; i < COUNT; ++i)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;

	t1.join();

	std::cout << g_global << std::endl;

	return 0;
}