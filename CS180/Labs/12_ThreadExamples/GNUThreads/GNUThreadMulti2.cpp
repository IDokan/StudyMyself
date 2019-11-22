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
    /*The Total number of threads I will create*/
    const int THREAD_COUNT = 5;
    /*When to stop incrementing*/
    const int INC_COUNT = 1000000;
    /*Shared global data*/
    int g_global = 0;
}
/*For POSIX threads we must return a void* and have one void* parameter*/
using ThreadFunc = void* (*)(void*);

void* Increment(void*)
{
    for (int i = 0; i < INC_COUNT; ++i)
        ++g_global;

    return nullptr;
}
void* Hello(void*)
{
    std::cout << "Hello" << std::endl;
    return nullptr;
}

int main(void)
{
    /*Array of thread ids to save*/
    pthread_t threadIDs[THREAD_COUNT];

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
        pthread_create(threadIDs + i, 0, pFunc[i], nullptr);
    }

    // wait for all threads to be done
    for (int i = 0; i < THREAD_COUNT; ++i)
        pthread_join(threadIDs[i], nullptr);


    /*global is corrupted because it it not thread safe*/
    std::cout << g_global << std::endl;

    return 0;
}
