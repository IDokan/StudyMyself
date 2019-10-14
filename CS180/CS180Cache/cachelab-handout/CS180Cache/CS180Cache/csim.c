/*
	Name: Kang ShinIl
	loginID: sinil.gang
*/
#include <stdlib.h>
#include <stdio.h>
#include "cachelab.h"

typedef struct {
	int s;
	int E;
	int b;
} cacheSetting;

int main(int argc, char* argv[])
{
	cacheSetting cache;
	char* traceFileName = 0;
	char option;
	while((option = getopt(argc, argv, "s:E:b:t:hv")) != -1)
	{
		switch(option)
		{
			case 's':
				cache.s = atoi(optarg);
			break;
			case 'E':
				cache.E = atoi(optarg);
			break;
			case 'b':
				cache.b = atoi(optarg);
			break;
			case 't':
				traceFileName = optarg;
			break;
			// What h stand for?
			case 'h':
			break;
			case 'v':
			break;
		}
	}

	FILE* traceFile = fopen(traceFileName, "r");
	if (traceFile != NULL)
	{

	}
    printSummary(cache.s, cache.E, cache.b);
    return 0;
}
