//
//  filehandling.c
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#include "filehandling.h"
#include "memory.h"

#define MAXCONST 64

static char* filebuffers[MAXCONST];
static unsigned count = 0;

//Create a new file, optionally read data into it, return the address to it in memory 
char* writeFile(const char* filename, const char* cont) {
	FILE* file = fopen(filename, "wb");
	//If contents param is null skip otherwise loop through each element of contents writing until a NULL terminator. 
	if (cont)
		for (; *cont; cont++) 
			fwrite((void*) cont, sizeof(char), 1, file);

	fclose(file);
	return readFile(filename);
}

//Writes a continous stream of characters to a file instead of a string
char* writeFileCont(const char* filename, size_t size, const char fillchar) {
	FILE* file = fopen(filename, "wb");
	for (; size > 0; size--)
		fwrite((void*) fillchar, sizeof(char), 1, file);

	fclose(file);
	return readFile(filename);
}

//Create an empty file
char* createFile(const char* filename) { return writeFile(filename, NULL); }

/*
Open a file and copy its contents to memory, return the address. The file data (in memory)
is writable but has no effect on the actual file data.
*/
char* readFile(const char* fileloc) {

	//If the maximum amount of files in memory is met: free the oldest piece of memory
	if (count == MAXCONST) {
		printerr(3, "File Buffer Full: overwriting files.\n");
		mem_free(filebuffers[0]);
		count = 0;
	}

	//Open the input file stream, read-only binary mode
	FILE* file = fopen(fileloc, "rb");

	//Check if the file was correctly opened
	if (file == NULL) {
		printerr(4, "File Open Error: file not found.\n");
		printf(fileloc);
		return NULL;
	}

	//Find the file length
	long int flen;
	if (!fseek(file, 0, SEEK_END)) {
		flen = ftell(file);
		rewind(file);
	}
	else {
		printerr(1, "File Seek Error.\n");
		return NULL;
	}

	//Reserve memory for buffer and read our file into it, 1 Byte is added as a terminator
	char* filebuffer = (char*) mem_malloc(flen + 1);
	filebuffer[flen] = '\0';

	//Read the file data into the buffer
	if (fread(filebuffer, sizeof(char), (size_t) flen, file) != flen) {
		printerr(2, "File Read Error.\n");
		return NULL;
	}

    //Close file stream
    fclose(file);
    
    //Add the address of our file buffer to our buffer list
    filebuffers[count] = filebuffer;
    
    //Increase count of file buffers
    /*
    If another file is added to the buffer list this value will be used
    to allocate the proper slot in the array for the next address
    */
    count++;
    
    return filebuffer;
}

//Close a specific file
void closeFile() {

}

//Free all files in memory
void closeFiles(void) {
    //Go down the buffer list freeing memory until we free the first entry;
    do {
		mem_free(filebuffers[count - 1]);
		filebuffers[count - 1] = NULL;
        count--;
    } while (count > 0);
}

//Retrieve file size
size_t stringSize(const char* string) {
	size_t size = 0;
	for (; *string; string++) size++;
	return size;
}
