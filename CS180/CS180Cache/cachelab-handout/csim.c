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

	int SetIndexMask;
	int TagBitsMask;
	int BlockOffsetMask;
} cacheSetting;


typedef struct
{
	int hits;
	int misses;
	int evictions;
} cacheResult;

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

void RunCache(const cache& myCache, unsigned long int address, const cacheSetting& myCacheSetting, result& myResult)
{

}

set* MakeCache(int numOfSets, numOfLines, numOfBlocks)
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

	return newCache.sets;
}

inline int PowerOf2(int i)
{
	return 1 << i;
}

int main(int argc, char* argv[])
{
	cacheSetting myCacheSetting;
	char* traceFileName = 0;
	char option;
	// Get Option and Set
	while((option = getopt(argc, argv, "s:E:b:t:hv")) != -1)
	{
		switch(option)
		{
			case 's':
				myCacheSetting.s = atoi(optarg);
			break;
			case 'E':
				myCacheSetting.E = atoi(optarg);
			break;
			case 'b':
				myCacheSetting.b = atoi(optarg);
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
	// Caculate Mask bits
	myCacheSetting.SetIndexMask = (1<<myCacheSetting.s) - 1;
	myCacheSetting.TagBitsMask = ((1<<(64 - (myCacheSetting.s + myCacheSetting.b))) - 1) ^ myCacheSetting.SetIndexMask;
	myCacheSetting.BlockOffsetMask = (-1) ^ (myCacheSetting.SetIndexMask + myCacheSetting.TagBitsMask);

	cache myCache;
	myCache.sets = MakeCache(PowerOf2(myCacheSetting.s), myCacheSetting.E, PowerOf2(myCacheSetting.b));


	cacheResult result;
	result.hits = 0;
	result.misses = 0;
	result.evictions = 0;

	char instructor;
	int address;
	int size;
	// Open Trace file and run
	FILE* traceFile = fopen(traceFileName, "r");
	if (traceFile != NULL)
	{
		while(fscanf(traceFile, " %c %x, %i", &instructor, &address, &size))
		{
			switch(instructor)
			{
				// Modify
				case 'M':

				break;
				// Load
				case 'L':
				break;
				// Store
				case 'S':
				break;
			}
		}
	}
	// Print s, E, b to DEBUG whether it is working or not.
    printSummary(cache.s, cache.E, cache.b);
    fclose(traceFile);
    ClearCache(myCache.sets);
    return 0;
}
