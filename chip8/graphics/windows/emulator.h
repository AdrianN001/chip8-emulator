#pragma once


#include "chip8/state/state.h"

#include <ncurses.h>


WINDOW* init_emulator_window(WINDOW* base_window);
void    update_emulator_window(WINDOW* window, state_t* state);