#include "chip8/graphics.h"
#include "chip8/state.h"
 


void draw_to_screen(WINDOW* window, state_t* state){
    wclear(window);
    for (int y = 0; y < 32; y++){
        for (int x = 0; x < 64; x++){
            int current_index = y*64 +x;
            bool draw_pixel = state->display_buffer[current_index];
            if (draw_pixel){
                mvwaddch(window, y+1, x, '#');
            }
        }
    }
    mvwprintw(window, 50, 50, "Current operation code: 0x%X\n", state->current_opcode);
    mvwprintw(window, 52, 50, "Next operation code: 0x%X\n", state->next_opcode);
    for (int i = 0; i < 16; i++){
        mvwprintw(window, i, 70, "V%X: %03X", i, state->gp_register[i]);
    } 
    mvwprintw(window, 30, 70, "SP: %03X", state->stack_pointer);
    mvwprintw(window, 32, 70, "I: %03X", state->I);
    mvwprintw(window, 34, 70, "PC: %03X", state->program_counter);
    wrefresh(window);
}
