#pragma once

#ifndef memory_h
#define memory_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* < Memory Buffer Information > */
size_t mem_getsize(void); //Returns amount of elements in the buffer

/* < Memory Buffer Operations > */
//Resize the memory array. Returns new size. Erases entries from end if shrinking.
size_t mem_resize(size_t newsize); 

/* < Standard Memory Operations > */
void* mem_calloc(const size_t Count, const size_t Size);
void* mem_malloc(const size_t Size);
void mem_free(void*); 

/* < Memory Clear Operation > */
//Frees all memory and resizes the buffer to 0. Returns new size (should be 0).
size_t mem_clear(void); 

#endif // !memory_h
