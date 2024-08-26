#include "chip8/graphics/windows.h"
#include "chip8/assert.h"

#include <ncurses.h>

WINDOW* init_debug_window(WINDOW* base_window){

    const int x_top = 160;
    const int y_top = 5;
    const int height = 50;
    const int width  = 35;


    WINDOW* new_window = subwin(stdscr, height, width, y_top, x_top);
    assertf(new_window != NULL, "couldnt create debug subwin");

    box(new_window, 0, 0);
    mvwprintw(new_window, 0, 12, "|Debug panel|");
    wrefresh(new_window);


    return new_window;
}

void update_debug_window(WINDOW* window, state_t* state){

    mvwprintw(window, 30, 2, "curr operation code: 0x%X", state->current_opcode);
    mvwprintw(window, 32, 2, "next operation code: 0x%X", state->next_opcode);
    for (int i = 0; i < 16; i++){
        mvwprintw(window, i+3, 2, "V%X: 0x%03X", i, state->gp_register[i]);
    } 
    mvwprintw(window, 20, 2, "SP: 0x%03X", state->stack_pointer);
    mvwprintw(window, 21, 2, "I:  0x%03X",  state->I);
    mvwprintw(window, 22, 2, "PC: 0x%03X", state->program_counter);
    wrefresh(window);
}
