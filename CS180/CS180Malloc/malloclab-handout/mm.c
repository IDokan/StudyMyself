/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please


 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Sinil Kang",
    /* First member's email address */
    "sinil.gang@digipen.edu",
    /* Second member's full name (leave blank if none) */
    "Jeesoo Kim",
    /* Second member's email address (leave blank if none) */
    "jeesoo.kim@digipen.edu"
};

/************************************************************************
 *					Rough Sketch for block description			      * 
 1. Allocated block
 ------------------------------------------------------------------------
 |Header|  						Payload					 |Padding|Footer|
 ------------------------------------------------------------------------
 2. Freed block
 ------------------------------------------------------------------------
 |Header|Predecessor|Successor|			Garbage data			 |Footer|
 ------------------------------------------------------------------------


 *) Header and Footer
 ------------------------------------------------------------------------
 |								Size					 	   |Allocate|
 ------------------------------------------------------------------------


************************************************************************/

/* Mine helper MACRO */

#define SIZEOFSEGREGATEDLIST 10;

#define TAG 0x7

#define PRED_PTR(ptr) ((char*)(ptr))
#define SUCC_PTR(ptr) ((char*)(ptr)+WSIZE)

#define PRED(ptr) (*((char**)(ptr)))
#define SUCC(ptr) (*((char**)SUCC_PTR(ptr)))

#define ALLOCATED	1
#define FREED		0

/* END of helper MACRO*/

/* Global Variables */

void* segregatedFreeList[SIZEOFSEGREGATEDLIST];

/* End of global variables */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Figure 9.43 : Basic constants and macros for manipulating the free list */

#define	WSIZE	4 /* Word and header / footer size (bytes) */
#define	DSIZE	8 /* Double word size (bytes) */
#define	CHUNKSIZE	(1 << 12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)	((size) | (alloc))

/* They are worked only if they are allocated */
/* Read and write a word at address p */
#define GET(p)	(*(unsigned int *)(p))
#define PUT(p, val)	(*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)	(GET(p) & ~0x7)
#define GET_ALLOC(p)	(GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)	((char *)(bp) - WSIZE)
#define FTRP(bp)	((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)	((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)	((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* End of Figure 9.43 */

// Helper functions

static int IsEpilogue(void* ptr)
{
	if (ptr == NULL)
	{
		return NULL;
	}

	return (GET_SIZE(HDRP(ptr)) == 0) && (GET_ALLOC(HDRP(ptr)) == ALLOCATED);
}

static void* extendHeap(size_t size)
{
	// make argument adjusted size, and store it in local variable.
	size_t adjustedSize = ALIGN(size);
	
	// call mem_sbrk(adjusted size), if return value is -1, it is error
	void* resultPtr = mem_sbrk(adjustedSize);
	if ((long)resultPtr == -1)
	{
		return NULL;
	}

	// Set Header and Footer
	PUT(HDRP(resultPtr), PACK(adjustedSize, 0));
	PUT(FTRP(resultPtr), PACK(adjustedSize, 0));
	PUT(HDRP(NEXT_BLKP(resultPtr)), PACK(0, 1));
	// Insert node to memory which allocated in here
	/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
	/* TODO: is it adjustedSize or size?????????? */
	/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */
	InsertNode(resultPtr, adjustedSize);

	// return return value of coalesce();
	return Coalesce(resultPtr);
}

static void InsertNode(void* ptr, size_t size)
{
	// Select segregated list
	size_t selectedIndex = GetIndex(size);

	// If index is invalid,
	if (selectedIndex >= SIZEOFSEGREGATEDLIST)
	{
		return;
	}

	// Get a list that listed proper nodes
	void* selectedList = segregatedFreeList[selectedIndex];

	// search correct block followed given size in segregated free list 
	void* previousNode = nullptr;
	void* nextNode = selectedList;
	while(nextNode != NULL && size > GET_SIZE(HDRP(nextNode)))
	{
		previousNode = nextNode;
		nextNode = SUCC(nextNode);
	}

	// If both previous node and next node are not NULL,
	if (previousNode != NULL && nextNode != NULL)
	{
		// PUT is okay??? I'm not sure...
		// For now PUT(p, val)	(*(unsigned int *)(p) = (val)),
		// May val be casted into (unsigned int)
		PUT(PRED_PTR(ptr), previousNode);
		PUT(SUCC_PTR(previousNode), ptr);
		PUT(SUCC_PTR(ptr), nextNode);
		PUT(PRED_PTR(nextNode), ptr);
	}
	// If previous node is NOT NULL but next node is NULL,
	else if(previousNode != NULL && nextNode == NULL)
	{
		PUT(PRED_PTR(ptr), previousNode);
		PUT(SUCC_PTR(previousNode), ptr);
		PUT(SUCC_PTR(ptr), NULL);
	}
	// If previous node is NULL, next node is NOT NULL,
	else if(previousNode == NULL && nextNode != NULL)
	{
		PUT(PRED_PTR(ptr), NULL);
		PUT(SUCC_PTR(ptr), nextNode);
		PUT(PRED_PTR(nextNode), ptr);
		segregatedFreeList[selectedIndex] = ptr;
	}
	// If both previous node and next node are NULL,
	else if(previousNode == NULL && nextNode == NULL)
	{
		PUT(PRED_PTR(ptr), NULL);
		PUT(SUCC_PTR(ptr), NULL);
		segregatedFreeList[selectedIndex] = ptr;
	}

	return;
}

static void* Coalesce(void* ptr)
{

	// Get a flag that indicates previous block and next block is allocated or not
	int isPreviousAllocated = GET_ALLOC(PREV_BLKP(ptr));
	int isNextAllocated = GET_ALLOC(NEXT_BLKP(ptr));
	// Get a Size of pointer
	size_t size = GET_SIZE(HDRP(ptr));

	/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */
	/* previous block referred as allocated if ★reallocation tag★ is also true */
	/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */

	// CASE 1
	// If both previous block and next block both allocated,
	if(isPreviousAllocated == ALLOCATED && isNextAllocated == ALLOCATED)
	{
		// Return immediately
		return ptr;
	}

	// CASE 2
	// If previous block is allocated and next block is freed,
	else if(isPreviousAllocated == ALLOCATED && isNextAllocated == FREED)
	{
		// delete node of ptr and next block of ptr
		deleteNode(NEXT_BLKP(ptr));
		deleteNode(ptr);
		// add size of next block of pointer into current size
		size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
		// Update pointers' header and footer with updated size
		PUT(FTRP(NEXT_BLKP(ptr)), 	PACK(size, 0));
		PUT(HDRP(ptr), 				PACK(size, 0));
	}

	// CASE 3
	// if previous block is freed and next block is allocated,
	else if(isPreviousAllocated == FREED && isNextAllocated == ALLOCATED)
	{
		// delete node of ptr and previous block of ptr
		deleteNode(PREV_BLKP(ptr));
		deleteNode(ptr);

		// add size of previous block of pointer into current size
		size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
		// Update pointers' header and footer with updated size
		PUT(FTRP(ptr),				PACK(size, 0));
		PUT(HDRP(PREV_BLKP(ptr)),	PACK(size, 0));		

		// Update pointer to make pointer points to previous block of pointed block
		ptr = PREV_BLKP(ptr);
	}

	// CASE 4
	// if both blocks are freed
	else if(isPreviousAllocated == FREED && isNextAllocated == FREED)
	{
		// delete node of ptr, previous block of ptr, and next block of ptr
		deleteNode(PREV_BLKP(ptr));
		deleteNode(NEXT_BLKP(ptr));
		deleteNode(ptr);
		// add size of both blocks into current size
		size += (GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(HDRP(NEXT_BLKP(ptr))));
		// Update pointers' header and footer with updated size
		PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
		// Update pointer to make pointer points to previous block of pointed block
		ptr = PREV_BLKP(ptr);
	}

	// Insert node pointer with size
	InsertNode(ptr);

	// return pointer
	return ptr; 
}

static void deleteNode(void* ptr)
{
	size_t selectedIndex = GetIndex(GET_SIZE(HDRP(ptr)));
	// Get a PRED and SUCC 
	void* predecessor = PRED(ptr);
	void* successor = SUCC(ptr);

	// If predecessor is NULL and successor is NULL,
	if (predecessor == NULL && successor == NULL)
	{
		// Set NULL the list which ptr was.
		segregatedFreeList[selectedIndex] = NULL;
	}
	// If predecessor is NOT NULL and successor is NULL,
	else if(predecessor != NULL && successor == NULL)
	{
		// Set predecessor's succesor ptr as NULL
		PUT(SUCC_PTR(predecessor), NULL);
	}
	// If predecessor is NULL and successor is NOT NULL,
	else if(predessor == NULL && successor != NULL)
	{
		// Set successor's predecessor as NULL
		PUT(PRED_PTR(successor), NULL);
		// Set successor as starting node
		segregatedFreeList[selectedIndex] = successor;
	}
	// If both is NOT NULL
	else if(predecessor != NULL && successor != NULL)
	{
		// connect each others
		PUT(SUCC_PTR(predecessor), successor);
		PUT(PRED_PTR(successor), predecessor);
	}
}

size_t GetIndex(size_t size)
{
	int bit16, bit8, bit4, bit2, bit1;

	bit16 = !!(x >> 16) << 4;
	x = x >> bit16;

	bit8 = !!(x >> 8) << 3;
	x = x >> bit8;

	bit4 = !!(x >> 4) << 2;
	x = x >> bit4;

	bit2 = !!(x >> 2) << 1;
	x = x >> bit2;

	bit1 = !!(x >> 1);
	x = x >> bit1;

	return bit16 + bit8 + bit4 + bit2 + bit1;
}

void place(void* ptr, size_t wantedSize)
{
	size_t sizeOfFreeBlock = GET_SIZE(HDRP(ptr));
	size_t leftSize = sizeOfFreeBlock - wantedSize;

	// delete ptr in segregated free list
	deleteNode(ptr);

	// If remain padding is too many,
	// 2 * DSIZE derived from (Header, Footer, payload)
	if (leftSize < 2*DSIZE)
	{
		// No need to split
		PUT(HDRP(ptr), PACK(sizeOfFreeBlock, ALLOCATED));
		PUT(FTRP(ptr), PACK(sizeOfFreeBlock, ALLOCATED));
	}
	else
	{
		PUT(HDRP(ptr), PACK(wantedSize, ALLOCATED));
		PUT(FTRP(ptr), PACK(wantedSize, ALLOCATED));
		PUT(HDRP(NEXT_BLKP(ptr)), PACK(leftSize, FREED));
		PUT(FTRP(NEXT_BLKP(ptr)), PACK(leftSize, FREED));
		InsertNode(NEXT_BLKP(ptr), leftSize);
	}
	return ptr;
}

// End of Helper functions

/* 
 * mm_init - initialize the malloc package.
 *
 * Verbose description :
 * Before calling mm_malloc mm_realloc or mm_free, 
 * the application program calls mm_init to perform any necessary initializations,
 * such as allocating the initial heap area.
 * The return value should be -1 if there was a probelm in performing the initialization, 0 otherwise.
 */
int mm_init(void)
{

	void * heapListP = NULL;
	
	// Clean segregated free list
	for (int i = 0; i < SIZEOFSEGREGATEDLIST; ++i)
	{
		segregatedFreeList[i] = NULL;
	}

	// allocate memory for start position of our heap via mem_sbrk();
	// If return value of mem_sbrk() == -1, it is a error. Thus, return -1
	if((heapListP = mem_sbrk(4*WSIZE)) == (void *) -1)
	{
		return -1;
	}

	// Set Prologue block, Epilogue block
	PUT(heapListP, 0);		// Alighment padding because the size of Epilogue block is not a DSIZE
	PUT(heapListP + (1*WSIZE), PACK(DSIZE, 1));	// Prologue header
	PUT(heapListP + (2*WSIZE), PACK(DSIZE, 1));	// Prologue footer
	PUT(headListP + (3*WSIZE), PACK(0, 1));		// Epilogue header
	heapListP += (2*WSIZE);		// Make heapListP point to payload of Prologue blocks

	// If the return value of extend_heap is NULL, return -1
	if(extend_heap(CHUNKSIZE) == NULL)
	{
		return -1;
	}
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 *
 * Verbose description :
 * The mm_malloc routine returns a pointer to an allocated block payload of at least size bytes.
 * The entire allocated block should lie within the heap region and should no overlap with any other allocated chunk.
 */
void *mm_malloc(size_t size)
{
    size_t aSize; 		/* Adjusted block size */
    size_t extendSize;	/* Amount to extend heap if no fit */
    char* resultPtr;	/* Pointer that will be returned */

    /* Ignore sprious requests */
    if(size == 0)
    {
    	return NULL;
    }

    /* Adjust block size to include overhead and alighment reqs. */
    if (size <= DSIZE)
    {
    	// TODO: Figure out why adjust size should be two times of DSIZE
    	aSize = 2 * DSIZE;
    }
    else
    {
    	aSize = (size + (DSIZE) + (DSIZE - 1)) & ~TAG;
    }

    /* Find a block with Segregate List */
    		/* if found block is not null, break */

    /* get a proper size first */
    size_t selectedIndex = GetIndex(aSize);
    resultPtr = segregatedFreeList[selectedIndex];

    /* get a precise block that has applicable size in the free list by walking */
	/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */
	/* Is it okay to search only in selected index?? */
	/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
    while(resultPtr != NULL && aSize > GET_SIZE(HDRP(resultPtr)))
    {
    	resultPtr = SUCC(resultPtr);
    }

    if (resultPtr != NULL)
    {
    	place(resultPtr, aSize);
    	return resultPtr;
    }

    /* No fit found. Get more memory and place the block */
    extendSize = MAX(aSize, CHUNKSIZE);
			/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
			/* TODO: extendSize / WSIZE ????? */
			/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */
    if ((resultPtr = extendHeap(extendSize / WSIZE)) == NULL)
    {
    	return NULL;
    }
    place(resultPtr, aSize);
    return resultPtr;
}

/*
 * mm_free - Freeing a block does nothing.
 *
 * Verbose description :
 * The mm_free routine frees the block pointed to by ptr.
 * It returns nothing.
 * This routine is only guaranteed to work when the passed pointer (ptr) was returned 
 * by an earlier call to mm_malloc or mm_realloc and has not yet been freed.
 */
void mm_free(void *ptr)
{
	size_t size = GET_SIZE(HDRP(ptr));

	// remove realloc tag
	PUT(HDRP(ptr), PACK(size, 0));
	PUT(FTRP(ptr), PACK(size, 0));

	// Insert it in the segregated free list

	coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 *
 * Verbose description :
 * The mm_realloc routine returns a pointer to an allocated region of at least size bytes with the following constraints.
 * - if ptr is NULL, the call is equivalent to mm_malloc(size);
 * - if size is equal to zero, the call is equivalent to mm_free(ptr);
 * - if ptr is not NULL, it must have been returned by an earlier call to mm_malloc or mm_realloc.
 *     The call to mm_realloc changes the size of the memory block pointed to by ptr (the old block) to size bytes and returns the address of the new block.
 *     Notice that the address of the new block might be the same as the old block, or it might be different, 
 *     depending on your implementation, the amount of internal fragmentation in the old block, and the size of the realloc request.
 *   The contents of the new block are the same as those of the old ptr block, up to the minimum of the old and new sizes.
 *     Everything else is uninitialized.
 *     For example, if the old block is 8 bytes and the new block is 12 bytes of the new block are identical to the first 8 bytes of the old block and the last 4 bytes are uninitialized.
 *     Similarly, if the old block is 8 bytes and the new block is 4 bytes, then the contens of the new block are identical to the first 4 bytes of the old block.
 */
void *mm_realloc(void *ptr, size_t size)
{
	// handle for defined situation
	if (ptr == NULL)
	{
		return mm_malloc(size);
	}
	if (size == 0)
	{
		mm_free(ptr);
		return ;
	}

	/* Align the given size */
    if (size <= DSIZE)
    {
    	size = 2 * DSIZE;
    }
    else
    {
    	size = (size + (DSIZE) + (DSIZE - 1)) & ~TAG;
    }

	size_t currentSize = GET_SIZE(HDRP(ptr));
	// size > size of ptr
	if (size > currentSize)
	{
		// check next block.
		// CASE 1 : Epilogue block
		void* nextBlock = NEXT_BLKP(ptr);
		if (IsEpilogue(nextBlock))
		{
			/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
			/* TODO: chunk size is okay or remaning size ????? */
			/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */
			if (extendHeap(CHUNKSIZE) == NULL)
			{
				return NULL;
			}
			size_t leftSize = size - currentSize;

			deleteNode(nextBlock);

			// Update header & footer with wanted size
			PUT(HDRP(ptr), PACK(size, ALLOCATED));
			PUT(FTRP(ptr), PACK(size, ALLOCATED));

			// Update header & footer of remaining free block and insert it.
			PUT(HDRP(NEXT_BLKP(ptr)), PACK(CHUNKSIZE - leftSize, FREED));
			PUT(FTRP(NEXT_BLKP(ptr)), PACK(CHUNKSIZE - leftSize, FREED));

			InsertNode(NEXT_BLKP(ptr));
		}
		// CASE 2 : Free block
		else if(GET_ALLOC(HDRP(nextBlock)) == FREED)
		{
			void* oldPtr = ptr;

			Coalesce(ptr);

			size_t coalescedSize = GET_SIZE(HDRP(ptr));
			// If coalesced size is big enough,
			if (coalescedSize >= size)
			{
				deleteNode(ptr);


				PUT(HDRP(ptr), PACK(size, ALLOCATED));
				PUT(FTRP(ptr), PACK(size, ALLOCATED));

				// ★
				size_t remainSize = coalescedSize - size;

				// If remaining size is big enough to store data,
				if (remainSize >= 2 * DSIZE)
				{
					PUT(HDRP(NEXT_BLKP(ptr)), PACK(remainSize, FREED));
					PUT(FTRP(NEXT_BLKP(ptr)), PACK(remainSize, FREED));

					InsertNode(NEXT_BLKP(ptr));
				}

				// If coalesced ptr is different with origin pointer,
				if (oldPtr != ptr)
				{
					// Move original data into coalesced pointer
					memcpy(ptr, oldPtr, GET_SIZE(HDRP(oldPtr)));
				}
			}
			// else, we should call mm_malloc();
			// Notice that the address of the new block might be the same as the old block.
			else
			{
				void* ptr = mm_malloc(size);
				memcpy(ptr, oldPtr, GET_SIZE(HDRP(oldPtr)));
			}
		}
		// CASE 3 : Allocated block
		else if(GET_ALLOC(HDRP(nextBlock)) == ALLOCATED)
		{
			ptr = mm_malloc(size);
			memcpy(ptr, oldPtr, GET_SIZE(HDRP(oldPtr)));
			mm_free(ptr);
		}
	}
	// size <= size of ptr
}