/******************************************************************************
filename    GNUThread1.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180

Brief Description:
Shows how to create a multipil thread using POSIX Threads.

******************************************************************************/
#include <cstdio>  /* printf                  */
#include <cstdlib> /*rand, srand*/
#include <windows.h>

namespace
{
    /*The number of threads to create*/
    const int THREAD_COUNT = 8;
    /*Shared global data between all threads*/
    char const* Table[] = {"A man, a plan, a canal: Panama!",
                           "No lemons, no melon.",
                           "Step on no pets.",
                           "Was it a rat I saw?",
                           "Dog as a devil deified lived as a god.",
                           "Able was I ere I saw Elba.",
                           "Yawn! Madonna fan? No damn way!",
                           "Go hang a salami. I'm a lasagna hog!"};

    /*The size of the table*/
    const int TableSize = sizeof(Table) / sizeof(*Table);

}

/*For Windows threads we must return a DWORD and have one void* parameter*/
using ThreadFunc = DWORD(WINAPI*)(void*);

DWORD WINAPI Palindrome(void*)
{
    const int MAX = 5;
    int       i, processID;

    /*Get thread ID*/
    int threadID = GetCurrentThreadId();
    /*seed rand with ThreadID*/
    srand((unsigned)threadID);

    /*Get process ID*/
    processID = GetCurrentProcessId(); /* all threads have the same pid */

    /*Print a message from the Table*/
    for (i = 0; i < MAX; ++i)
    {
        int index = rand() % TableSize;
        printf("[p:%i][t:%u]: %s\n", processID, (int)threadID, Table[index]);
        Sleep(1000); /*Sleep for one second*/
    }

    return 0;
}

int main(void)
{
    /*Array of Thread ids to save*/
    HANDLE threads[THREAD_COUNT];

    /*A function pointer to call*/
    ThreadFunc pFunc = Palindrome;


    printf("Creating threads...\n");

    /*Create all my threads*/
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        threads[i] = CreateThread(0,     /*Default Attributes*/
                                  0,     /*Default Stack size*/
                                  pFunc, /*Function pointer*/
                                  0,     /*Parameters*/
                                  0,     /*Creation flags*/
                                  0      /*Returned threadID*/
        );
    }
    printf("Done creating threads.\n");

    printf("Waiting on threads...\n");
    /*Make sure to wait for all threads*/
    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);
    printf("Done waiting on threads.\n");

    /*close all thread handles */
    for (int i = 0; i < THREAD_COUNT; ++i)
        CloseHandle(threads[i]);

    return 0;
}