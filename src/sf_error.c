#include <stdio.h>

void sf_print_error(const int i, const char* msg) {
	printf("Error %d: %s\n", i, msg);
}

void sf_print_debug(const char* msg) {
	printf("%s\n", msg);
}