/******************************************************************************
filename    CPPThread1.cpp
author      Jookyung Lee, Sinil Gang
DP email    jookyung.lee@digipen.edu, sinil.gang@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using CPP Threads.

******************************************************************************/

#include <iostream>
#include <thread>

void Hello()
{
	std::cout << "Hello!\n";
	return;
}

int main(void)
{
	std::thread t1{Hello};

	t1.join();

	return 0;
}