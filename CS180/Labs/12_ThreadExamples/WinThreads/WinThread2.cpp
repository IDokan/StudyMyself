/******************************************************************************
filename    WinThread2.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180


Brief Description:
  Example of using WINDOWS threads and sharing global data

******************************************************************************/
#include <iostream>  /*cout*/
#include <windows.h> /*threads*/

namespace
{
    /*Our increment count*/
    const int COUNT    = 100000;
    int       g_global = 0; /*The global varaible to count*/
}

/*For Windows threads we must return a DWORD and have one void* parameter*/
using ThreadFunc = DWORD(WINAPI*)(void*);

DWORD WINAPI Increment(LPVOID)
{
    /*Loop and increment out global variable*/
    for (int i = 0; i < COUNT; ++i)
        ++g_global;

    return 0; /*Just return 0*/
}


int main(void)
{
    HANDLE     thread;            /*To save our thread*/
    ThreadFunc pFunc = Increment; /*To save our function*/

    /*Create the seperate thread*/
    thread = CreateThread(0,     /*Default Attributes*/
                          0,     /*Default Stack size*/
                          pFunc, /*Function pointer*/
                          0,     /*Parameters*/
                          0,     /*Creation flags*/
                          0      /*Returned threadID*/
    );


    /*Our main thread can be busy with other stuff*/
    for (int i = 0; i < COUNT; ++i)
        std::cout << i << " ";

    std::cout << std::endl;

    /*Use this to wait for the thread to finish*/
    WaitForSingleObject(thread,  /*Therad handle*/
                        INFINITE /*Wait time in millisecons, INFINITE is forever*/
    );

    /*We must close the thread handle*/
    CloseHandle(thread);

    std::cout << g_global << std::endl;

    return 0;
}
