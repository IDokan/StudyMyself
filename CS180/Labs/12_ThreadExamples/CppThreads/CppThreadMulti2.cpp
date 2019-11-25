/******************************************************************************
filename    CPPThreadMulti2.cpp
author      Jookyung Lee, Sinil Gang
DP email    jookyung.lee@digipen.edu, sinil.gang@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using CPP Threads.

******************************************************************************/
#include <iostream>
#include <thread>
#if _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace
{
    /*The Total number of threads I will create*/
    const int THREAD_COUNT = 5;
    /*When to stop incrementing*/
    const int INC_COUNT = 1000000;
    /*Shared global data*/
    int g_global = 0;
}
/*For Windows threads we must return a void* and have one void* parameter*/
using ThreadFunc = void(*)(void);

void Increment(void)
{
	for (int i = 0; i < INC_COUNT; ++i)
	{
		++g_global;
	}
	return;
}
void Hello()
{
	std::cout << "Hello" << std::endl;
	return;
}

int main(void)
{
	// Array of thread ids to save
	std::thread threads[THREAD_COUNT];

	// function pointers
	ThreadFunc pFunc[THREAD_COUNT];

	// Threads can point to different functions
	pFunc[0] = Hello;
	pFunc[1] = Increment;
	pFunc[2] = Increment;
	pFunc[3] = Increment;
	pFunc[4] = Increment;

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads[i] = std::thread{pFunc[i]};
	}

	// wait for all threads to be done
	for (int i = 0; i < THREAD_COUNT; ++i)
		{
			threads[i].join();
		}	

	// global is corrupted because it is not thread safe
		std::cout << g_global << std::endl;

	return 0;
}