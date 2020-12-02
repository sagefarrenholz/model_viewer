#ifndef interleave_h
#define interleave_h

#include "memory.h"

//Interleave vertex arrays into one array to pass to a buffer object
float* interleave(const float* vertex, const float* texture, size_t arraysize) {
	float* arraybuff = mem_calloc(arraysize, sizeof(float));
	int v = 0, t = 0;
	/*
	Loop through the entire array buffer and add vertex values if 
	at one of the first 3 coordinates of a line. If at the last 2
	(3,4) then use then next texture values.
	*/
	if(arraysize * 3.0 / 2.0)
	for (size_t i = 0; i < arraysize; i++) {
		if (i % 5 < 3) {
			arraybuff[i] = vertex[v];
			v++;
		} else {
			arraybuff[i] = texture[t];
			t++;
		}
	}
	return arraybuff;
}

#endif // !interleave.h
