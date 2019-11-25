/******************************************************************************
filename    CPPThreadData.cpp
author      Jookyung Lee, Sinil Gang
DP email    jookyung.lee@digipen.edu, sinil.gang@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using CPP Threads.

******************************************************************************/
#include <iostream>  /*cout*/
#include <thread> /*threads*/

namespace
{
	const int THREAD_COUNT = 5;

	const int INC_COUNT = 1000000;

	struct ThreadData
	{
		const char* printString;
		int count;
	};
}

using ThreadFunc = void(*)(void*);

void Increment(void* ptr)
{
	std::cout << "Doing Increment\n";

	ThreadData * pData = reinterpret_cast<ThreadData*>(ptr);
	for(int i = 0; i < INC_COUNT; ++i)
	{
		++(pData->count);
	}
	std::cout << "Finished Increment\n";
	return;
}

void Hello(void* ptr)
{
	ThreadData * pData = reinterpret_cast<ThreadData*>(ptr);
	std::cout << pData->printString << std::endl;
	return;
}

int main()
{
	ThreadFunc pFunc[THREAD_COUNT];

	ThreadData data;

	data.printString = "Hello World";
	data.count = 0;

	pFunc[0] = Hello;
	pFunc[1] = Increment;
	pFunc[2] = Increment;
	pFunc[3] = Hello;
	pFunc[4] = Increment;

	std::thread threadIDs[THREAD_COUNT] = 
	{
		std::thread(pFunc[0], &data),
		std::thread(pFunc[1], &data),
		std::thread(pFunc[2], &data),
		std::thread(pFunc[3], &data),
		std::thread(pFunc[4], &data)
	};

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threadIDs[i].join();
	}

std::cout << data.count << std::endl;

	return 0;
}