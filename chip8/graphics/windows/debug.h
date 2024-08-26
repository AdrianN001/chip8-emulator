#pragma once

#include "chip8/state/state.h"

#include <ncurses.h>


WINDOW* init_debug_window(WINDOW* base_window);
void    update_debug_window(WINDOW* window, state_t* state);