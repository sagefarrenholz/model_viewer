#ifndef sf_error_h
#define sf_error_h

enum {
	SF_UNSPECIFIED_ERROR = 2000
};

void sf_print_error(int error_code, const char* msg);
void sf_print_debug(const char* msg);

#endif // !sf_error_h
