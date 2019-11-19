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

static void* extendHeap(size_t size)
{
	// make argument adjusted size, and store it in local variable.
	
	// call mem_sbrk(adjusted size), if return value is -1, it is error

	// Set Header and Footer
	// Insert node to memory which allocated in here

	// return return value of coalesce();
	// Argument is an allocated memory 

	//////////////////////////////////////////////////////////

	char* resultPtr;
	size_t aSize;

	/* Allocate an even number of words to maintain alighment */
	aSize = (size % 2) ? (size + 1) * WSIZE : size * WSIZE;
	if((long)(resultPtr = mem_sbrk(size)) == -1)
	{
		return NULL;
	}

	/* Initialize free block header / footer and the epilogue header */
	PUT(HDRP(resultPtr), PACK(size, 0));			/* Free block header */
	PUT(FTRP(resultPtr), PACK(size, 0));			/* Free block footer */
	PUT(HDRP(NEXT_BLKP(resultPtr)), PACK(0, 1));	/* New epilogue header */

}

static void insertNode(void* ptr, size_t size)
{
	// Select segregated list
	// In this case, result value should store whole size

	// search correct block followed given size in segregated free list 
	// In this loop, you should find search_ptr, insert_ptr

	// If both search_ptr and insert_ptr are not NULL,
	// ...
	// If search_ptr is NOT NULL but insert_ptr is NULL,
	// ...
	// If search_ptr is NULL, insert_ptr is NOT NULL,
	// ...
	// If both search_ptr and inser_ptr are NULL,
	// ... 
}

static void* coalesce(void* ptr)
{
	// Get a flag that indicates previous block and next block is allocated or not
	// Get a Size of pointer

	// previous block referred as allocated if reallocation tag is also true

	// CASE 1
	// If both previous block and next block both allocated
	// Return immediately

	// CASE 2
	// If previous block is allocated and next block is freed
	// delete node of ptr and next block of ptr
	// add size of next block of pointer into current size
	// Update pointers' header and footer with updated size
		// Notice that they are segregated header and footer!!!

	// CASE 3
	// if previous block is freed and next block is allocated
	// delete node of ptr and previous block of ptr
	// add size of previous block of pointer into current size
	// Update pointers' header and footer with updated size
		// Notice that they are segregated header and footer!!!
	// Update pointer to make pointer points to previous block of pointed block

	// CASE 4
	// if both blocks are freed
	// delete node of ptr, previous block of ptr, and next block of ptr
	// add size of both blocks into current size
	// Update pointers' header and footer with updated size
		// Notice that they are segregated header and footer!!!
	// Update pointer to make pointer points to previous block of pointed block

	// Insert node pointer with size

	// return pointer 
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
	PUT(heapListP, 0);		// Alighment padding
	PUT(heapListP + (1*WSIZE), PACK(DSIZE, 1));	// Prologue header
	PUT(heapListP + (2*WSIZE), PACK(DSIZE, 1));	// Prologue footer
	PUT(headListP + (3*WSIZE), PACK(0, 1));		// Epilogue header
	heapListP += (2*WSIZE);

	// If the return value of extend_heap is NULL, return -1
	if(extend_heap(CHUNKSIZE/WSIZE) == NULL)
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
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }

/////////////////////////////////////////////////////////////

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
    	// TODO: Is it same with ((size+(DSIZE)+(DSIZE-1)) & ~TAG)?
    	aSize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);
    }

    // It will be obsolete, when segregated is worked
    /* Search the free list for a fit */
    if ((resultPtr = find_fit(aSize)) != NULL)
    {
    	place(resultPtr, aSize);
    	return bp;
    }

    /* Find a block with Segregate List */
    	/* get a proper size first */
    		/* if found, get a precise block in the free list by walking */
    		/* if found block is not null, break */

    /* No fit found. Get more memory and place the block */
    extendSize = MAX(aSize, CHUNKSIZE);
    if ((resultPtr = extend_heap(extendSize / WSIZE)) == NULL)
    {
    	return NULL;
    }
    place(bp, aSize);
    return bp;
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
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;

    ///////////////////////////////////////////////////////////////////

    /* If size is 0, ignore it */

    /* Align block size */
    /* ???????????????? */

    /* Add overhead requirements to block size */
    /* ??????????????????????????????????????? */

    /* Calculate block buffer */
    /* ?????????????????????? */
}














