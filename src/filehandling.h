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
#include "errorhandling.h"

char* openFile(const char* fileloc);
void closeFiles(void);

#endif /* filehandling_h */
