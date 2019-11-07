# Implicit Free List Lab

For this lab you will practice looping through an Implicit Free List.

Provided are 5 snapshots of different instances of a heap stored with an Implicit Free List.

Word size is 4.

Each payload has a Header and matching Footer that stores the size and if it is allocated.

All heaps start with an allocated payload 8 bytes. All heaps end with an allocated payload of 0 bytes.

All memory provided to the application is a multiple of 8.

You need to figure out the following properties for each heap snapshot:

Gross Heap Size      | Total size of all allocations (does include the size of the OVERHEAD)
Total Provided Size  | Total size provided to the application (so it does not include the size of the OVERHEAD)
free total size      | How much total space is free for the application
allocated total size | How much total space is being used by the application
free count           | How many payloads are free
allocated count      | How many payloads are allocated

You are to write a program to help you calculate this information. It must do the following:

1. Load a provided file using `mmap()` to get a pointer to the beginning of the file.
2. Write an algorithm to walk the free list, while collecting the needed information.
3. Print out the info.


## Submission

Submit the properties for each heap and submit the code you wrote to generate the information.
