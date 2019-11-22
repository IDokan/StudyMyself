/******************************************************************************
filename    GNUThread1.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using POSIX Threads.

******************************************************************************/
/* To compile under Linux, use -pthread or -lpthread option */
#include <iostream>  /* cout                    */
#include <pthread.h> /* thread create/join/exit */

/*We can set functions to run on seperate threads
A function must have return a void pointer and take a void pointer as
an argument.*/
void* Hello(void*)
{
    std::cout << "Hello!\n";
    return nullptr; /*Void Pointer return status*/
}

int main(void)
{
    /*Create a thread id*/
    pthread_t threadID;

    /*Create the seperate thread*/
    pthread_create(&threadID, /*Our thread ID*/
                   0,         /*Atributes about the thread, we don't care*/
                   Hello,     /*Our fucntion pointer*/
                   nullptr);  /*Void* parameter.*/

    /*Use this to */
    pthread_join(threadID, nullptr /*void** to get the return status.*/
    );

    return 0;
}