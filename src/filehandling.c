//
//  filehandling.c
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#include "filehandling.h"

static const unsigned MAXCONST = 10;

static char* filebuffers[MAXCONST];
static unsigned count = 0;

char* openFile(const char* fileloc) {
    
    //If the maximum amount of files in memory is met: free the oldest piece of memory
    if(count == MAXCONST) {
        printerr(3, "WARNING OVERWRITING FILES\n");
        free(filebuffers[0]);
        count = 0;
    }
    
    //Open the input file stream
    FILE* file = fopen(fileloc, "r");
    
    //Check if the file was correctly opened
    if(file == NULL) {
        printerr(4, "FILE OPEN ERROR: ");
        printf(fileloc);
        assert(1);
    }
    
    //Find the file length
    fpos_t flen;
    if(!fseek(file, 0, SEEK_END)){
        fgetpos(file, &flen);
        rewind(file);
    } else {
        printerr(1, "FILE SEEK ERROR\n");
        return NULL;
    }
    
    //Reserve memory for buffer and read our file into it
    char* filebuffer = (char*) calloc(flen, 1);
    if(fread(filebuffer, 1, flen, file) != flen) {
        printerr(2, "FILE READ ERROR\n");
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

void closeFiles(void) {
    //Go down the buffer list freeing memory until we free the first entry;
    do {
        free(filebuffers[count]);
        count--;
    } while (count > 0);
}
