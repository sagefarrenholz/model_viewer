#ifndef sf_error_h
#define sf_error_h

enum {
	SF_UNSPECIFIED_ERROR = 2000
};

void sf_print_error(int error_code, const char* msg);


#ifndef _DEBUG 
#define SF_DEBUG(x, ...)
#else
#define SF_DEBUG printf
#endif
void sf_print_debug(const char* msg);

#endif // !sf_error_h
