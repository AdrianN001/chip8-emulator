#pragma once
#include <stdint.h>
#include "register.h"
#include "display.h"

typedef struct { 
    
    uint8_t memory[4* 1024]; 
    reg_t gp_register[16]; // General Purpose Registers

    uint8_t delay_timer_register;
    uint8_t sound_timer_register;

    uint16_t I;
    uint16_t program_counter;
    
    uint16_t stack[16];
    uint8_t  stack_pointer;

    display_state_t display_buffer[64*32];

    uint8_t key_pad[16];
    uint16_t current_opcode;
    uint16_t next_opcode;
    int draw_flag;

} state_t;

state_t init_state();
void read_program(state_t* state, uint8_t* buffer, long size);

void emulate_cycle(state_t* state);