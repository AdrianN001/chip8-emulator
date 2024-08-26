#include "chip8/graphics/windows.h"
#include "chip8/graphics.h"
#include "chip8/state.h"

void draw_to_screen(WINDOW* emulator_window, WINDOW* debug_window, state_t* state){
    update_debug_window(debug_window, state);
    update_emulator_window(emulator_window, state);
}
