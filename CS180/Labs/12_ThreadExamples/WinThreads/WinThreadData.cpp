/******************************************************************************
filename    WinThreadMulti2.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180

Brief Description:
Shows how to create a multiple thread using Windows Threads.

******************************************************************************/
#include <iostream> /* cout                  */
#include <windows.h>

namespace
{
    /*The Total number of threads I will create*/
    const int THREAD_COUNT = 5;
    /*When to stop incrementing*/
    const int INC_COUNT = 1000000;

    struct ThreadData
    {
        const char* printString;
        int         count;
    };
}
/*For Windows threads we must return a void* and have one void* parameter*/
using ThreadFunc = DWORD(WINAPI*)(void*);

DWORD WINAPI Increment(void* ptr)
{
    ThreadData* pData = reinterpret_cast<ThreadData*>(ptr);
    for (int i = 0; i < INC_COUNT; ++i)
        ++(pData->count);

    return 0;
}
DWORD WINAPI Hello(void* ptr)
{
    ThreadData* pData = reinterpret_cast<ThreadData*>(ptr);
    std::cout << pData->printString << std::endl;
    return 0;
}

int main(void)
{
    /*Array of thread ids to save*/
    HANDLE threads[THREAD_COUNT];

    /*function pointers*/
    ThreadFunc pFunc[THREAD_COUNT];

    /*Shared Data*/
    ThreadData data;

    data.printString = "Hello World";
    data.count       = 0;

    /*Threads can point to different functions*/
    pFunc[0] = Hello;
    pFunc[1] = Increment;
    pFunc[2] = Increment;
    pFunc[3] = Hello;
    pFunc[4] = Increment;


    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        threads[i] = CreateThread(0,        /*Default Attributes*/
                                  0,        /*Default Stack size*/
                                  pFunc[i], /*Function pointer*/
                                  &data,    /*Parameters*/
                                  0,        /*Creation flags*/
                                  0         /*Returned threadID*/
        );
    }

    /*wait for all threads to be done*/
    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);


    /*global is corrupted because it it not thread safe*/
    std::cout << data.count << std::endl;

    return 0;
}
