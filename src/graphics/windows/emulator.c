#include "chip8/graphics/windows.h"
#include "chip8/assert.h"


WINDOW* init_emulator_window(WINDOW* base_window){

    const int x_top = 50;
    const int y_top = 5;
    const int height = 50;
    const int width  = 92;


    WINDOW* new_window = subwin(base_window, height, width, y_top, x_top);

    assertf(new_window != NULL, "couldnt create emulator subwin");

    box(new_window, 0, 0);

    wrefresh(new_window);

    return new_window;
}

void update_emulator_window(WINDOW* window, state_t* state){
    wclear(window);
    box(window, 0, 0);
    for (int y = 0; y < 32; y++){
        for (int x = 0; x < 64; x++){
            int current_index = y*64 +x;
            bool draw_pixel = state->display_buffer[current_index];
            if (draw_pixel){
                mvwaddch(window, y+9, x+14, '#');
            }
        }
    }
    wrefresh(window);
}