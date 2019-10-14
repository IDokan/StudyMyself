/*
	Name: Kang ShinIl
	loginID: sinil.gang
*/
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "cachelab.h"

typedef struct {
	int s;
	int E;
	int b;
} cacheSetting;

typedef struct 
{
	bool valid;
	int accessCount;
	int tag;
	bool* block;
} line;

typedef struct
{
	line* lines;
} set;

typedef struct
{
	set* sets;
} cache;

cache MakeCache(int numOfSets, numOfLines, numOfBlocks)
{
	cache newCache;
	newCache.sets = (set*)malloc(numOfSets*sizeof(set));
	for (int countSet = 0; countSet < numOfSets; ++countSet)
	{
		newCache.sets[countSet].lines = (line*)malloc(numOfLines*sizeof(line));

		for (int countLine = 0; countLine < numOfLines; ++countLine)
		{
			newCache.sets[countSet].lines[countLine].valid = false;
			newCache.sets[countSet].lines[countLine].accessCount = 0;
			newCache.sets[countSet].lines[countLine].tag = 0;
			newCache.sets[countSet].lines[countLine].block = (bool*)malloc(numOfBlocks);
		}
	}

	return newCache;
}

inline int PowerOf2(int i)
{
	return 1 << i;
}

int main(int argc, char* argv[])
{
	cacheSetting cacheOption;
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

	cache myCache;
	myCache.sets = MakeCache(PowerOf2(cacheOption.s), cacheOption.E, PowerOf2(cacheOption.b));

	FILE* traceFile = fopen(traceFileName, "r");
	if (traceFile != NULL)
	{

	}
	// Print s, E, b to DEBUG whether it is working or not.
    printSummary(cache.s, cache.E, cache.b);
    return 0;
}
