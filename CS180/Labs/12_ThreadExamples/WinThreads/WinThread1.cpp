/******************************************************************************
filename    WinThread1.cpp
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using Windows Threads.

******************************************************************************/
#include <iostream>  /* cout                  */
#include <windows.h> /*CreateThread, WaitForSingleObject, CloseHandle*/

/*We can set functions to run on seperate threads
A function must have return a DWORD and take a void pointer as
an argument LPVOID is the same as void* .*/
DWORD WINAPI Hello(LPVOID /*ptr*/)
{
    std::cout << "Hello!\n";
    return 0; /*Just return 0*/
}

int main(void)
{
    /*Create a thread id*/
    HANDLE thread;

    /*Create the seperate thread*/
    thread = CreateThread(0,     /*Default Attributes*/
                          0,     /*Default Stack size*/
                          Hello, /*Function pointer*/
                          0,     /*Parameters*/
                          0,     /*Creation flags*/
                          0      /*Returned threadID*/
    );


    /*Use this to wait for the thread to finish*/
    WaitForSingleObject(thread,  /*Therad handle*/
                        INFINITE /*Wait time in millisecons, INFINITE is forever*/
    );

    /*We must close the thread handle*/
    CloseHandle(thread);

    return 0;
}