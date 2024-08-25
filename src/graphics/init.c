#include <stdlib.h>

#include "chip8/graphics.h"


WINDOW* init_base_window(){
    ESCDELAY = 0; // remove delay after pressing escape


    WINDOW* main_window;
    if ( (main_window = initscr()) == NULL ) {
	    fprintf(stderr, "Error initialising ncurses.\n");
	    exit(1);
    }
    curs_set(0); /* Hide the cursor */
    cbreak();
    noecho();
    nodelay(main_window, true);
    return main_window;
}