#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "chip8/state.h"
#include "chip8/rand.h"
#include "chip8/assert.h"

state_t init_state(){
    srand(time(NULL));
    
    state_t state;

    state.program_counter = 512;
    state.I = 0x0;
    state.stack_pointer = 0;

    memset(state.memory, 0, 4096);
    memset(state.display_buffer, 0, 64*32);

    for (int i = 0; i< 80; i++){
        state.memory[i] = chip8_fontset[i];
    }

    for (int i = 0; i < 16; i++){
        state.gp_register[i] = 0;
    }

    for (int i = 0; i < 16; i++){
        state.stack[i] = 0;
    }


    return state;
}

void read_program(state_t* state, uint8_t* buffer, long size){
    for (int i = 0; i < size; i++){
        state->memory[i+512] = buffer[i];
    }
}

void emulate_cycle(state_t* state){
    uint16_t opcode = state->memory[state->program_counter] << 8 | state->memory[state->program_counter+1];
    state->current_opcode = opcode;

    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x00FF)
            {
            case 0x00E0: // 0x00E0: Clears the screen        
                memset(state->display_buffer, 0, 64*32);
                state->draw_flag = 1;
                state->program_counter += 2;
            break;
        
            case 0x00EE: // 0x00EE: Returns from subroutine          
                state->program_counter = state->stack[state->stack_pointer-1];
                state->stack_pointer--;
                state->program_counter += 2;
            break;
        
            default:
                printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
            }
        break;
        
        case 0x1000: // 1NNN: JP addr
            state->program_counter = opcode & 0x0FFF;
        break;

        case 0x2000: // 2NNN: CALL addr
            state->stack[state->stack_pointer] = state->program_counter;
            state->stack_pointer++;
            state->program_counter = opcode & 0x0FFF;
        break;

        case 0x3000: // 3XKK: SE Vx, byte
            if (state->gp_register[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                state->program_counter += 4;
            }else
                state->program_counter += 2;
        break;

        case 0x4000: // 4XKK: SNE Vx, byte
            if (state->gp_register[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                state->program_counter += 4; 
            }else
                state->program_counter += 2;
        break;

        case 0x5000: // 5xy0: SE Vx, Vy
            if (state->gp_register[(opcode & 0x0F00) >> 8] == state->gp_register[(opcode & 0x00F0) >> 4]){
                state->program_counter += 4; 
            }else
                state->program_counter += 2;
        break;

        case 0x6000: // 6xKK: LD Vx, byte
            state->gp_register[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            state->program_counter += 2;
        break;
        
        case 0x7000: // 7xKK: ADD Vx, byte
            state->gp_register[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            state->program_counter += 2;
        break;


        case 0x8000:
            switch (opcode & 0x00F){
                case 0x0000://  0x8XY0: LD Vx, Vy
                    state->gp_register[(opcode & 0x0F00) >> 8] = state->gp_register[(opcode & 0x00F0) >> 4];
                    state->program_counter += 2;               
                break;
                case 0x0001://  0x8XY1: OR Vx, Vy
                    state->gp_register[(opcode & 0x0F00) >> 8] = state->gp_register[(opcode & 0x0F00) >> 8] | state->gp_register[(opcode & 0x00F0) >> 4];
                    state->program_counter += 2;                
                break;
                case 0x0002://  0x8XY2: AND Vx, Vy
                    state->gp_register[(opcode & 0x0F00) >> 8] = state->gp_register[(opcode & 0x0F00) >> 8] & state->gp_register[(opcode & 0x00F0) >> 4];
                    state->program_counter += 2;                
                break;
                case 0x0003://  0x8XY3: XOR Vx, Vy
                    state->gp_register[(opcode & 0x0F00) >> 8] = state->gp_register[(opcode & 0x0F00) >> 8] ^ state->gp_register[(opcode & 0x00F0) >> 4];
                    state->program_counter += 2;                
                break;
                case 0x0004:{ //  0x8XY4: ADD Vx, Vy
                    uint8_t x_register_val = state->gp_register[(opcode & 0x0F00) >> 8];
                    uint8_t y_register_val = state->gp_register[(opcode & 0x00F0) >> 4];
                    state->gp_register[0xF] = (x_register_val + y_register_val > 255);
                    state->gp_register[(opcode & 0x0F00) >> 8] = (x_register_val + y_register_val) & 0x00FF;
                    state->program_counter += 2;
                }
                break;
                case 0x0005:{ // 0x8XY5: SUB Vx, Vy
                    uint8_t x_register_val = state->gp_register[(opcode & 0x0F00) >> 8];
                    uint8_t y_register_val = state->gp_register[(opcode & 0x00F0) >> 4];
                    state->gp_register[0xF] = (x_register_val > y_register_val);
                    state->gp_register[(opcode & 0x0F00) >> 8] -= state->gp_register[(opcode & 0x00F0) >> 4];
                    state->program_counter += 2;
                }
                break;
                case 0x0006:{ //  0x8XY6: SHR Vx {, Vy}
                    int least_significant_bit = (state->gp_register[(opcode & 0x0F00) >> 8]) & 0x01;
                    state->gp_register[0xF] = least_significant_bit;
                    state->gp_register[(opcode & 0x0F00) >> 8] >>= 1;
                    state->program_counter += 2;
                }
                break;
                case 0x007:{ // 0x8XY7: SUBM Vx, Vy
                    state->gp_register[0xF] = (state->gp_register[(opcode & 0x0F00) >> 8]) > (state->gp_register[(opcode & 0x00F0) >> 4]);
                    state->gp_register[(opcode & 0x0F00) >> 8] = state->gp_register[(opcode & 0x00F0) >> 4] - state->gp_register[(opcode & 0x0F00) >> 8];
                    state->program_counter += 2;
                }
                break;
                case 0x000E:{ // 0x8XYE: SHL Vx {, Vy}
                    int most_significant_bit = (state->gp_register[(opcode & 0x0F00) >> 8] >> 7) & 0x01;
                    state->gp_register[0xF] = most_significant_bit;
                    state->gp_register[(opcode & 0x0F00) >> 8] <<= 1;
                    state->program_counter += 2;
                }
                break;
                default:
					printf ("Unknown opcode [0x8000]: 0x%X\n", opcode);
            }
        break;

        case 0x9000:
            if (state->gp_register[(opcode & 0x0F00) >> 8] != state->gp_register[(opcode & 0x00F0) >> 4]){
                state->program_counter += 4;
            }else
                state->program_counter += 2;
        break;

        case 0xA000: // 0xANNN: Sets I to the address NNN
            state->I = (opcode & 0x0FFF);
            state->program_counter += 2;
        break;

        case 0xB000: // 0xBNNN: JP V0, addr
            state->program_counter = (opcode & 0x0FFF) + state->gp_register[0x0];
        break;

        case 0xC000:{ // 0xCxkk: RND Vx, byte
            uint8_t random_number = RAND(0, 255);
            state->gp_register[(opcode & 0x0F00) >> 8] = random_number & (opcode & 0x00FF);
            state->program_counter += 2;
        }
        break;

        case 0xD000:{ // 0xDxyn: DRW Vx, Vy, nibble
            uint8_t x = state->gp_register[(opcode & 0x0F00) >> 8];
			uint8_t y = state->gp_register[(opcode & 0x00F0) >> 4];
			uint8_t height = opcode & 0x000F;
			uint8_t pixel;

			state->gp_register[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = state->memory[state->I + yline];
				for(int xline = 0; xline < 8; xline++)
				{
					if((pixel & (0x80 >> xline)) != 0)
					{
						if(state->display_buffer[(x + xline + ((y + yline) * 64))] == 1)
						{
							state->gp_register[0xF] = 1;                                    
						}
						state->display_buffer[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}
			state->draw_flag = true;	
            state->program_counter += 2;
        }
        break;

        case 0xE000:
            switch(opcode & 0x00FF){
                case 0x009E: // 0xEx9E: SKP Vx
                    if (state->key_pad[ state->gp_register[(opcode & 0x0F00) >> 8]] != 0){
                        state->program_counter += 4;
                    }else
                        state->program_counter += 2;
                break;
                case 0x00A1: // 0xExA1: SKNP Vx
                    if (state->key_pad[ state->gp_register[(opcode & 0x0F00) >> 8]] == 0){
                        state->program_counter += 4;
                    }else
                        state->program_counter += 2;
                break; 
            }
        break;

        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007: // 0xFx07: LD Vx, DT
                    state->gp_register[(opcode & 0x0F00) >> 8] = state->delay_timer_register;
                    state->program_counter += 2;
                break;
                case 0x000A:{ // 0xFx0A: LD Vx, K
                    bool any_key_pressed = false;

					for(int i = 0; i < 16; ++i)
					{
						if(state->key_pad[i] != 0)
						{
							state->gp_register[(opcode & 0x0F00) >> 8] = i;
							any_key_pressed = true;
						}
					}

					// If we didn't received a keypress, skip this cycle and try again.
					if(!any_key_pressed)						
						return;
                    state->program_counter += 2;
                }
                break;
                case 0x0015: // 0xFx15: LD DT, Vx
                    state->delay_timer_register = state->gp_register[(opcode & 0x0F00) >> 8];
                    state->program_counter += 2;
                break;
                case 0x0018: // 0xFx18: LD ST, Vx
                    state->sound_timer_register = state->gp_register[(opcode & 0x0F00) >> 8];
                    state->program_counter += 2;
                break;
                case 0x001E: // 0xFx1E: ADD I, Vx
                    state->I += state->gp_register[(opcode & 0x0F00) >> 8];
                    state->program_counter += 2;
                break;
                case 0x0029: // 0xFx29: LD F, Vx
                    state->I = state->gp_register[(opcode & 0x0F00) >> 8] * 0x5;
                    state->program_counter += 2;
                break;
                case 0x0033: // 0xFx33: LD B, Vx
                    state->memory[state->I] = state->gp_register[(opcode & 0x0F00) >> 8] / 100;
                    state->memory[state->I + 1] = (state->gp_register[(opcode & 0x0F00) >> 8] / 10) % 10;
                    state->memory[state->I + 2] = (state->gp_register[(opcode & 0x0F00) >> 8] % 100) % 10;
                    state->program_counter += 2;
                break;
                case 0x0055: // 0xFx55: LD [I], Vx
                    for (int reg_p = 0; reg_p <= ((opcode & 0x0F00) >> 8); reg_p++){
                        state->memory[state->I+reg_p] = state->gp_register[reg_p];
                    }
                    state->I += ((opcode & 0x0F00) >> 8) + 1;
                    state->program_counter += 2;
                break;
                case 0x0065: // 0xFx65: LD Vx, [I]
                    for (int reg_p = 0; reg_p <= ((opcode & 0x0F00) >> 8); reg_p++){
                        state->gp_register[reg_p] = state->memory[state->I+reg_p];
                    }
                    state->I += ((opcode & 0x0F00) >> 8) + 1;
                    state->program_counter += 2;
                break;


				default:
					printf ("Unknown opcode [0xF000]: 0x%X\n", opcode);
            }

        break;


		default:
			printf ("Unknown opcode: 0x%X\n", opcode);
    }

    if (state->delay_timer_register > 0){
        state->delay_timer_register--;
    }
    if (state->sound_timer_register > 0){
        if (state->sound_timer_register == 1){
            // Should beep now
        }
        state->sound_timer_register--;
    }

    state->next_opcode = state->memory[state->program_counter] << 8 | state->memory[state->program_counter+1];
}