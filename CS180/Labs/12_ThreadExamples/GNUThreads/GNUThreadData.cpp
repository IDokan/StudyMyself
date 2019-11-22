/******************************************************************************
filename    GNUThreadMulti2.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180

Brief Description:
Shows how to create a multiple thread using POSIX Threads.

******************************************************************************/
#include <cstdlib>   /*rand, srand*/
#include <iostream>  /* cout                  */
#include <pthread.h> /* thread create/join/exit */
#include <unistd.h>  /* sleep, getpid           */

namespace
{
    const int THREAD_COUNT = 5;       /*The Total number of threads I will create*/
    const int INC_COUNT    = 1000000; /*When to stop incrementing*/

    struct ThreadData
    {
        const char* printString;
        int         count;
    };
}
/*For POSIX threads we must return a void* and have one void* parameter*/
using ThreadFunc = void* (*)(void*);

void* Increment(void* ptr)
{
    std::cout << "Doing Increment\n";
    ThreadData* pData = reinterpret_cast<ThreadData*>(ptr);
    for (int i = 0; i < INC_COUNT; ++i)
        ++(pData->count);

    std::cout << "Finished Increment\n";
    return nullptr;
}
void* Hello(void* ptr)
{
    ThreadData* pData = reinterpret_cast<ThreadData*>(ptr);
    std::cout << pData->printString << std::endl;
    return nullptr;
}

int main(void)
{
    pthread_t  threadIDs[THREAD_COUNT]; /*Array of thread ids to save*/
    ThreadFunc pFunc[THREAD_COUNT];     /*function pointers*/
    ThreadData data;                    /*My shared data to pass to the threads*/

    data.printString = "Hello World"; /*My string to print for the hello function*/
    data.count       = 0;             /*My Data to increment for the increment function*/

    /*Threads can point to different functions*/
    pFunc[0] = Hello;
    pFunc[1] = Increment;
    pFunc[2] = Increment;
    pFunc[3] = Hello;
    pFunc[4] = Increment;

    // Create my threads
    for (int i = 0; i < THREAD_COUNT; ++i)
        pthread_create(threadIDs + i, 0, pFunc[i], &data);

    // wait for all threads to be done
    for (int i = 0; i < THREAD_COUNT; ++i)
        pthread_join(threadIDs[i], nullptr);


    /*global is corrupted because it it not thread safe*/
    std::cout << data.count << std::endl;

    return 0;
}
