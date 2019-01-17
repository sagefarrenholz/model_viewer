//
//  errorhandling.c
//  opengl-test
//
//  Created by Sage Farrenholz on 1/16/19.
//  Copyright © 2019 Sage Farrenholz. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include "errorhandling.h"

void assert (int assertion) {
    if(assertion) exit(0);
}

void printerr (unsigned char errcode, const char* msg) {
    printf("ERROR %u: ", (unsigned) errcode);
    printf(msg);
}
