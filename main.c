#include <stdio.h>
#include <ncurses.h>
#include "chip8/graphics.h"
#include "chip8/state.h"

int main(int argc, char** argv){
    WINDOW* base_window = init_base_window();
    state_t ctx = init_state();
    load_application(&ctx, argv[1]);

    for (;;){
        
        emulate_cycle(&ctx);
        if (ctx.draw_flag){
            draw_to_screen(base_window, &ctx);
        }

        int char_pressed = wgetch(base_window);
        handle_keyboard_input(&ctx, char_pressed);
        napms(50);
    }
    return 0;
}