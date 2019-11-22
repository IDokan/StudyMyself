/******************************************************************************
filename    GNUThread2.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180


Brief Description:
  Example of using posix threads and sharing global data

******************************************************************************/
#include <iostream>  /*cout*/
#include <pthread.h> /*threads*/

namespace
{
    /*Our increment count*/
    const int COUNT    = 100000;
    int       g_global = 0; /*The global variable to count*/
}

/*For POSIX threads we must return a void* and have one void* parameter*/
typedef void* (*ThreadFunc)(void*);

void* Increment(void*)
{
    /*Loop and increment out global variable*/
    for (int i = 0; i < COUNT; ++i)
        ++g_global;

    return nullptr; /*Just return null*/
}


int main(void)
{
    pthread_t  threadID;          /*To save our thread id*/
    ThreadFunc pFunc = Increment; /*To save our function*/

    pthread_create(&threadID, /*Our threadID*/
                   0,         /*Attributes*/
                   pFunc,     /*A pointer to the function we want to run*/
                   nullptr    /*The parameter we want to pass*/
    );

    /*Our main thread can be busy with other stuff*/
    for (int i = 0; i < COUNT; ++i)
        std::cout << i << " ";

    std::cout << std::endl;

    /*We must wait until the thread has finsished*/
    pthread_join(threadID, /*Thread to wait for*/
                 nullptr   /*Return value from function (void**)*/
    );

    std::cout << g_global << std::endl;

    return 0;
}
