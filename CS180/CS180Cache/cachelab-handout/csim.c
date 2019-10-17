/*
	Name: Kang ShinIl
	loginID: sinil.gang
*/
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cachelab.h"

typedef struct {
	int s;
	int E;
	int b;

	// Number of sets and blocks
	int S;
	int B;

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
	int timeCount;
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

int GetEmptyIndex(const set& mySet, const cacheSetting& myCacheSetting)
{
	const int lineSize = myCacheSetting.E;
	for (int lineCount = 0; lineCount < lineSize; ++lineCount)
	{
		if (mySet.lines[lineCount].valid == 0)
		{
			return lineCount;
		}
	}
	return -1;
}

int GetLRU(const cache* myCache, const cacheSetting myCacheSetting)
{
	int leastRecentlyUsed = 99;
	int lineIndex = -1;
	for (int setCount = 0; setCount < myCacheSetting.S; ++setCount)
	{
		for (int lineCount = 0; lineCount < myCacheSetting.E; ++lineCount)
		{
			if (myCache.sets[setCount].lines[lineCount].timeCount < leastRecentlyUsed)
			{
				leastRecentlyUsed = myCache.sets[setCount].lines[lineCount].timeCount;
				lineIndex = lineCount;
			}
		}
	}
	return lineIndex;
}

void LoadCache(const cache* myCache, unsigned long int address, const cacheSetting myCacheSetting, cacheResult* myResult)
{
	const int selectedBlockOffset = (address & myCacheSetting.BlockOffsetMask);
	const int selectedSetIndex = (address & myCacheSetting.SetIndexMask) >> (myCacheSetting.b);
	const int selectedTagBits = (address & myCacheSetting.TagBitsMask) >> (myCacheSetting.b + myCacheSetting.s);

// !
	for (int setCount = 0; setCount < myCacheSetting.S; ++setCount)
	{
		for (int lineCount = 0; lineCount < myCacheSetting.E; ++lineCount)
		{
			++myCache->sets[setCount].lines[lineCount].timeCount;
		}
	}

	set selectedSet = myCache->sets[selectedSetIndex];
	for (int lineCount = 0; lineCount < myCacheSetting.E; ++lineCount)
	{
		if (selectedSet.lines[lineCount].valid == true)
		{
			if (selectedSet.lines[lineCount].tag == selectedTagBits)
			{
				// If hit, should be stop here
				++selectedSet.lines[lineCount].timeCount;
				++myResult->hits;
				return;
			}
		}
	}

	// If missed,
	int EmptyIndex = GetEmptyIndex(selectedSet, myCacheSetting);
	if (EmptyIndex == -1)
	{
		GetLRU(myCache);
	}
	else
	{
		selectedSet.lines[EmptyIndex].valid = true;
		++selectedSet.lines[EmptyIndex].timeCount;
		selectedSet.lines[EmptyIndex].tag = selectedTagBits;
		++myResult->misses;
	}
	return;
}

void StoreCache(const cache& myCache, unsigned long int address, const cacheSetting& myCacheSetting, result& myResult)
{
}

set* MakeCache(int numOfSets, int numOfLines, int numOfBlocks)
{
	set* newCache;
	newCache = (set*)malloc(numOfSets*sizeof(set));
	for (int countSet = 0; countSet < numOfSets; ++countSet)
	{
		newCache[countSet].lines = (line*)malloc(numOfLines*sizeof(line));

		for (int countLine = 0; countLine < numOfLines; ++countLine)
		{
			newCache[countSet].lines[countLine].valid = false;
			newCache[countSet].lines[countLine].timeCount = 0;
			newCache[countSet].lines[countLine].tag = 0;
			newCache[countSet].lines[countLine].block = (bool*)malloc(numOfBlocks);
		}
	}

	return newCache;
}

void ClearCache(set* deletedCache, int numOfSets, int numOfLines, int numOfBlocks)
{
	for (int countSet = 0; countSet < numOfSets; ++countSet)
	{
		for (int countLine = 0; countLine < numOfLines; ++countLine)
		{
			free(deletedCache[countSet].lines[countLine].block);
		}
		free(deletedCache[countSet].lines);
	}
	free(deletedCache);
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
	myCacheSetting.BlockOffsetMask = (1 << myCacheSetting.b) - 1;
	myCacheSetting.SetIndexMask = (1<<(myCacheSetting.s+myCacheSetting.SetIndexMask)) - 1;
	myCacheSetting.TagBitsMask = (-1) ^ (myCacheSetting.SetIndexMask + myCacheSetting.BlockOffsetMask);

	cache myCache;
	myCacheSetting.S = PowerOf2(myCacheSetting.s);
	myCacheSetting.B = PowerOf2(myCacheSetting.b);
	myCache.sets = MakeCache(myCacheSetting.S, myCacheSetting.E, myCacheSetting.B);


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
    printSummary(result.hits, result.misses, result.evictions);
    fclose(traceFile);
    ClearCache(myCache.sets, myCacheSetting.S, myCacheSetting.E, myCacheSetting.B);
    return 0;
}