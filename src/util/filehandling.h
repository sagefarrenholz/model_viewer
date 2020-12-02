//
//  filehandling.h
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#ifndef filehandling_h
#define filehandling_h

#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "errorhandling.h"

//Create a blank file and return a pointer to it.
char* createFile(const char* filename);

//Write to a file, if no file exists create it. Return a pointer to that written file.
char* writeFile(const char* filename, const char* content);
//Write a continous string of characters (fillchar), length len, to a file. Return a pointer to the file. 
char* writeFileCont(const char* filename, size_t len, const char fillchar);

//Read a file into the file buffer and return a pointer to that file.
char* readFile(const char* fileloc);


/* 
TODO:
Rewrite this function. Doing so relies on adding singular entry lookups 
in memory.c. Awaiting that functionality.
*/
//Close all files in the buffer.
//void closeFiles(void);

//Return size of a string (char array).
size_t stringSize(const char* string);

#endif /* filehandling_h */
