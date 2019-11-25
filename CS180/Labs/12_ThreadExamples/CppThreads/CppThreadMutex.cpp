/******************************************************************************
filename    CPPThreadMultiMutex.cpp
author      Jookyung Lee, Sinil Gang
DP email    jookyung.lee@digipen.edu, sinil.gang@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using CPP Threads.

******************************************************************************/
#include <iostream>
#include <thread>
#include <mutex>

namespace
{
    /*The Total number of threads I will create*/
    const int THREAD_COUNT = 5;
    /*When to stop incrementing*/
    const int INC_COUNT = 1000000;

    struct ThreadData
    {
        const char*      printString;
        int              count;
        std::mutex Mutex;
    };
}


/*For Windows threads we must return a void* and have one void* parameter*/
using ThreadFunc = void(*)(ThreadData*);

void Increment(ThreadData* data)
{
	{
		std::scoped_lock lock(data->Mutex);
		for (int i = 0; i < INC_COUNT; ++i)
		{
			++(data->count);
		}
	}
	return;
}

void Hello(ThreadData* data)
{
	std::cout << data->printString << std::endl;
	return;
}

int main()
{
	// Array of thread ids to save
	std::thread threads[THREAD_COUNT];

	// function pointers
	ThreadFunc pFunc[THREAD_COUNT];

	// Shared Data
	ThreadData data;

	data.printString = "Hello World";
	data.count = 0;

	pFunc[0] = Hello;
	pFunc[1] = Increment;
    pFunc[2] = Increment;
    pFunc[3] = Hello;
    pFunc[4] = Increment;

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
    	threads[i] = std::thread{pFunc[i], &data};
    }

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
    	threads[i].join();
    }

    std::cout << data.count << std::endl;

	return 0;
}