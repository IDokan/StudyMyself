/******************************************************************************
filename    CPPThreadMulti1.cpp
author      Jookyung Lee, Sinil Gang
DP email    jookyung.lee@digipen.edu, sinil.gang@digipen.edu
course      CS180

Brief Description:
Shows how to create a basic thread using CPP Threads.

******************************************************************************/
#include <iostream>  /*cout*/
#include <thread> /*threads*/
namespace
{
	const int THREAD_COUNT = 8;

	char const* Table[] = {"A man, a plan, a canal: Panama!",
                           "No lemons, no melon.",
                           "Step on no pets.",
                           "Was it a rat I saw?",
                           "Dog as a devil deified lived as a god.",
                           "Able was I ere I saw Elba.",
                           "Yawn! Madonna fan? No damn way!",
                           "Go hang a salami. I'm a lasagna hog!"};

	const int TableSize = sizeof(Table) / sizeof(*Table);
}

using ThreadFunc = void(*)();


int get_process_id();
void MySleep(int time);

#if _WIN32
#include <windows.h>
int get_process_id()
{
	return GetCurrentProcessId();
}
void MySleep(int time)
{
	Sleep(time*1000);
}
#else
#include <unistd.h>
int get_process_id()
{
	return getpid();
}
void MySleep(int time)
{
	sleep(time);
}
#endif


void Palindrome()
{
	const int MAX = 5;
	int 		i, processID;

	std::hash<std::thread::id> hasher;
	auto result = hasher(std::this_thread::get_id());

	srand((unsigned)result);

	processID = get_process_id();

	for (i = 0; i < MAX; ++i)
	{
		int index = rand() % TableSize;
		printf("[p:%i][t:%u]: %s\n", processID, (int)result, Table[index]);
		MySleep(1);
	}
	return;
}

int main(void)
{
	std::thread threadIDs[THREAD_COUNT];

	ThreadFunc pFunc = Palindrome;

	printf("Creating threads...\n");

	for(int i = 0; i < THREAD_COUNT; ++i)
	{
		threadIDs[i] = std::thread{pFunc};
	}
	printf("Done creating threads.\n");

	printf("Waiting on threads...\n");

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		printf("Joining thread %i\n", i);
		threadIDs[i].join();
	}
	printf("Done waiting on threads.\n");

	return 0;
}