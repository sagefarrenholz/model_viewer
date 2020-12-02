#include "sf.h"
#include "sf_error.h"

typedef struct _SF {
	SF_Window** win;
	int win_count;
	SF_Renderer* ren;
} SF;

static SF* sf_instance;

void sf_init(void) {

	sf_instance = (SF*) mem_malloc(sizeof(SF));
}

/*SF_Window* sf_add_window(SF_Window* window) {
	if (window == NULL) {
		sf_set_simple_window_hints();
		window = sf_init_window(-1, -1, NULL);
		if (window == NULL) {
			sf_print_error(SF_UNSPECIFIED_ERROR, "Could not intialize default window in sf_add_window");
		}
	} 
	return sf_instance->win[sf_instance->win_count++] = window;
}*/

void sf_quit(void) {
	mem_clear();
}