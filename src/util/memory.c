#include "memory.h"

//Start of the buffer in memory
static void** membuffer = NULL;
//How full the buffer is, subtract one for the index of the last element
static size_t size = 0;

/*
TODO:
Potentially implement binary search for free
*/

//Resize the memory array
size_t mem_resize(const size_t memcap) {
	//Free all entries beyond new end, if shrinking
	if (memcap < size) {
		for (size_t i = memcap; i < size; i++) {
			free(membuffer[i]);
		}
	}
	if (memcap) membuffer = (void*) realloc(membuffer, memcap * sizeof(void*));
	else free(membuffer);
	return size = memcap;
}

//Standard free wrapped by the memory buffer 
void mem_free(void* const loc) {
	if (membuffer[size - 1] != loc) {
		//Free the given pointer, only needed here because memResize also performs this.
		free(loc);
		//Linear search for specified pointer, swap the last entry into this slot, lower count by 1
		size_t i = 0;
		while(loc != membuffer[i]) i++;
		membuffer[i] = membuffer[size - 1];
		membuffer = (void*) realloc(membuffer, (size - 1) * sizeof(void*));
		size--;
	} else {
		// Frees and removes the last entry
		mem_resize(size - 1);
	}
}

//Standard calloc wrapped by the memory buffer
void* mem_calloc(const size_t Count, const size_t Size) {
	//Expand membuffer by one and allocate memory according to arguements, return pointer.
	mem_resize(size+1);
	return membuffer[size - 1] = calloc(Count, Size);
}

//Standard malloc wrapped by the memory buffer
void* mem_malloc(const size_t Size) {
	mem_resize(size+1);
	return membuffer[size - 1] = malloc(Size);
}

//Frees all memory and resizes the buffer to 0. Returns new size (should be 0).
size_t mem_clear(void) {
	return mem_resize(0);
}

//Returns how full the buffer is
size_t mem_getsize(void) { return size; }
