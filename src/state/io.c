#include "chip8/state.h"
#include "chip8/assert.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void load_application(state_t* state, char* filaneme){
    FILE* fp = fopen(filaneme, "rb");
    assertf(fp != NULL, "failed to open rom file");


	fseek(fp , 0 , SEEK_END);
	long file_size = ftell(fp);
	rewind(fp);
	printf("Filesize: %d\n", (int)file_size);


    uint8_t* buffer = malloc(sizeof(uint8_t) * file_size);
    assertf(buffer != NULL, "memory error");

    size_t result = fread (buffer, 1, file_size, fp);

	if((4096-512) > file_size){
		for(int i = 0; i < file_size; ++i){
			state->memory[i + 512] = buffer[i];
		}
	}

    fclose(fp);
    free(buffer);

}


void handle_keyboard_input(state_t* state, int keyboard_input){
	if (keyboard_input == ERR){
		for (int i = 0; i < 16; i++){
			state->key_pad[i] = 0; 
		}
	}else{
		
		if((char)keyboard_input == '1')		state->key_pad[0x1] = 1;
		else if((char)keyboard_input == '2')	state->key_pad[0x2] = 1;
		else if((char)keyboard_input == '3')	state->key_pad[0x3] = 1;
		else if((char)keyboard_input == '4')	state->key_pad[0xC] = 1;

		else if((char)keyboard_input == 'q')	state->key_pad[0x4] = 1;
		else if((char)keyboard_input == 'w')	state->key_pad[0x5] = 1;
		else if((char)keyboard_input == 'e')	state->key_pad[0x6] = 1;
		else if((char)keyboard_input == 'r')	state->key_pad[0xD] = 1;

		else if((char)keyboard_input == 'a')	state->key_pad[0x7] = 1;
		else if((char)keyboard_input == 's')	state->key_pad[0x8] = 1;
		else if((char)keyboard_input == 'd')	state->key_pad[0x9] = 1;
		else if((char)keyboard_input == 'f')	state->key_pad[0xE] = 1;

		else if((char)keyboard_input == 'z')	state->key_pad[0xA] = 1;
		else if((char)keyboard_input == 'x')	state->key_pad[0x0] = 1;
		else if((char)keyboard_input == 'c')	state->key_pad[0xB] = 1;
		else if((char)keyboard_input == 'v')	state->key_pad[0xF] = 1;


	}
}