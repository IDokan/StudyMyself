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
    /*Shared global data*/
    int g_global = 0;
}
/*For Windows threads we must return a void* and have one void* parameter*/
using ThreadFunc = DWORD(WINAPI*)(void*);

DWORD WINAPI Increment(void*)
{
    for (int i = 0; i < INC_COUNT; ++i)
        ++g_global;

    return 0;
}
DWORD WINAPI Hello(void*)
{
    std::cout << "Hello" << std::endl;
    return 0;
}

int main(void)
{
    /*Array of thread ids to save*/
    HANDLE threads[THREAD_COUNT];

    /*function pointers*/
    ThreadFunc pFunc[THREAD_COUNT];

    /*Threads can point to different functions*/
    pFunc[0] = Hello;
    pFunc[1] = Increment;
    pFunc[2] = Increment;
    pFunc[3] = Increment;
    pFunc[4] = Increment;


    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        threads[i] = CreateThread(0,        /*Default Attributes*/
                                  0,        /*Default Stack size*/
                                  pFunc[i], /*Function pointer*/
                                  0,        /*Parameters*/
                                  0,        /*Creation flags*/
                                  0         /*Returned threadID*/
        );
    }

    /*wait for all threads to be done*/
    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);


    /*global is corrupted because it it not thread safe*/
    std::cout << g_global << std::endl;

    return 0;
}
