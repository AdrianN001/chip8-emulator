#pragma once
#include <ncurses.h>
#include "chip8/state/state.h"


void draw_to_screen(WINDOW* window, state_t* state);