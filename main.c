#include <stdio.h>
#include <ncurses.h>
#include "chip8/graphics/windows.h"
#include "chip8/graphics.h"
#include "chip8/state.h"

int main(int argc, char** argv){
    WINDOW* base_window = init_base_window();
    state_t ctx = init_state();
    load_application(&ctx, argv[1]);

    WINDOW* debug_window = init_debug_window(base_window);
    WINDOW* emulator_window = init_emulator_window(base_window);


    for (;;){
        emulate_cycle(&ctx);
        if (ctx.draw_flag){
            draw_to_screen(emulator_window, debug_window, &ctx);
        }

        int char_pressed = wgetch(base_window);
        handle_keyboard_input(&ctx, char_pressed);
        napms(25);
    }
    
    return 0;
}