/******************************************************************************
filename    GNUThreadMulti1.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180

Brief Description:
Shows how to create a multipile thread using POSIX Threads.

******************************************************************************/
#include <cstdio>    /* printf                  */
#include <cstdlib>   /* rand, srand             */
#include <pthread.h> /* thread create/join/exit */
#include <unistd.h>  /* sleep, getpid           */

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

/*For POSIX threads we must return a void* and have one void* parameter*/
using ThreadFunc = void* (*)(void*);

void* Palindrome(void*)
{
    const int MAX = 5;
    int       i, processID;

    /*Get thread ID*/
    pthread_t threadID = pthread_self();
    /*seed rand with ThreadID*/
    srand((unsigned)threadID);

    /*Get process ID*/
    processID = getpid(); /* all threads have the same pid */

    /*Print a message from the Table*/
    for (i = 0; i < MAX; ++i)
    {
        int index = rand() % TableSize;
        printf("[p:%i][t:%u]: %s\n", processID, (unsigned)threadID / 1000, Table[index]);
        sleep(1); /*Sleep for one second*/
    }

    return nullptr;
}

int main(void)
{
    /*Array of Thread ids to save*/
    pthread_t threadIDs[THREAD_COUNT];

    /*A function pointer to call*/
    ThreadFunc pFunc = Palindrome;


    printf("Creating threads...\n");

    /*Create all my threads*/
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        pthread_create(&threadIDs[i], /*The id of the created thread*/
                       0,             /*Thread attributes*/
                       pFunc, nullptr /*Function parameters*/
        );
    }

    printf("Done creating threads.\n");

    printf("Waiting on threads...\n");
    /*Make sure to wait for all threads*/
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        printf("Joining thread %i\n", i);
        pthread_join(threadIDs[i], 0);
    }
    printf("Done waiting on threads.\n");

    return 0;
}