#pragma once

#include "Types.h"
#include "FC.h"

fn InitInterpreter() {
	
	// setting program counter to zero
	PC = 0;

	// getting flags
	for (i32 i = 0; i < 512; i++) {
		if (ROM[i] == ASM_FLG)
			FC.Flags[ROM[i + 1]] = i;

		if (ROM[i] != ASM_NOP &&
			ROM[i] != ASM_INP &&
			ROM[i] != ASM_OUT &&
			ROM[i] != ASM_INC && 
			ROM[i] != ASM_DEC)
			i++;
	}
}

fn Interpreter() {
	f32 sleep = 0;

	while (sleep < 1000 / FRAMERATE) {

		// shortcuts
		i8 command = ROM[PC];
		i8 arg = ROM[PC + 1];

		// no operation
		if (command == ASM_NOP)
			pass


		// PIN MANAGEMENT
		
		// change pin
		else if (command == ASM_PIN)
			sleep += asm_pin(arg);

		// get from pin
		else if (command == ASM_INP)
			sleep += asm_inp();

		// send to pin
		else if (command == ASM_OUT)
			sleep += asm_out();


		// REGISTER MANAGEMENT
		
		// load immidient
		else if (command == ASM_LDI)
			sleep += asm_ldi(arg);

		// load register
		else if (command == ASM_LDA)
			sleep += asm_lda(arg);

		// store to register
		else if (command == ASM_STA)
			sleep += asm_sta(arg);


		// MATH
		
		// increment accumulator
		else if (command == ASM_INC)
			sleep += asm_inc();

		// decrement accumulator
		else if (command == ASM_DEC)
			sleep += asm_dec();

		// and accumulator with register
		else if (command == ASM_AND)
			sleep += asm_and(arg);

		// and accumulator with register
		else if (command == ASM_XOR)
			sleep += asm_xor(arg);

		// compare register and accumulator
		else if (command == ASM_CMP)
			sleep += asm_cmp(arg);


		// PROGRAM MANAGEMENT
		
		// skip defined count of nibbles
		else if (command == ASM_SKP)
			sleep += asm_skp(arg);

		// put a flag
		else if (command == ASM_FLG)
			PC++;

		// jump to flag
		else if (command == ASM_JMP)
			sleep += asm_jmp(arg);


		// HALT

		// can't read more than memory
		PC++;
		if (PC >= 512)
			Quit("Out of commands.");
	}
}


f32 asm_pin(arg) {
	PIN = arg;
	PC++;

	return TIME_PIN;
}

f32 asm_inp() {

	// returns from memory bank3
	if (PIN == PIN_MEMORY1) {
		if (FC.Memory.Sends[2] == 2) {
			ACC = ROM[FC.Memory.Sends[0] + FC.Memory.Sends[1] * 16];
			FC.Memory.Sends[2] = 0;
		}
		else
			Quit("Can't get input from ROM1, some of parameters ungiven.");
	}

	// returns from memory bank2
	else if (PIN == PIN_MEMORY2) {
		if (FC.Memory.Sends[5] == 2) {
			ACC = ROM[FC.Memory.Sends[3] + FC.Memory.Sends[4] * 16 + 256];
			FC.Memory.Sends[5] = 0;
		}
		else
			Quit("Can't get input from ROM2, some of parameters ungiven.");
	}

	// returns keyboard input
	else if (PIN == PIN_KEYBOARD) {
		ACC =
			(FC.Key.Down  << 3) +
			(FC.Key.Up    << 2) +
			(FC.Key.Right << 1) +
			FC.Key.Left;
	}

	// can't return a screen
	else if (PIN == PIN_SCREEN)
		Quit("Can't get input from screen.");

	return TIME_INP;
}

f32 asm_out() {

	// sets in memory bank 1
	if (PIN == PIN_MEMORY1) {
		if (FC.Memory.Sends[2] == 2) {
			ROM[FC.Memory.Sends[0] + FC.Memory.Sends[1] * 16] = ACC;
			FC.Memory.Sends[2] = 0;
		}
		else
			FC.Memory.Sends[FC.Memory.Sends[2]++] = ACC;
	}

	// sets in memory bank 2
	else if (PIN == PIN_MEMORY2) {
		if (FC.Memory.Sends[5] == 2) {
			ROM[FC.Memory.Sends[3] + FC.Memory.Sends[4] * 16 + 256] = ACC;
			FC.Memory.Sends[5] = 0;
		}
		else
			FC.Memory.Sends[FC.Memory.Sends[5]++ + 3] = ACC;
	}

	// flips pixel on screen
	else if (PIN == PIN_SCREEN) {
		if (FC.Screen.Sends[2] == 1) {
			SetPixel(FC.Screen.Sends[0], ACC);
			FC.Screen.Sends[2] = 0;
		}
		else
			FC.Screen.Sends[FC.Screen.Sends[2]++] = ACC;
	}

	// can't send to a keyboard
	else if (PIN == PIN_KEYBOARD)
		Quit("Can't set output for keyboard.");

	return TIME_OUT;
}


f32 asm_ldi(arg) {
	ACC = arg;
	PC++;

	return TIME_LDI;
}

f32 asm_lda(arg) {
	ACC = REG[arg];
	PC++;

	return TIME_LDA;
}

f32 asm_sta(arg) {
	REG[arg] = ACC;
	PC++;

	return TIME_STA;
}


f32 asm_inc() {
	ACC = (ACC + 1) & 0b1111;

	return TIME_INC;
}

f32 asm_dec() {
	ACC = (ACC - 1) & 0b1111;

	return TIME_DEC;
}

f32 asm_and(arg) {
	ACC &= REG[arg];
	PC++;

	return TIME_AND;
}

f32 asm_xor(arg) {
	ACC ^= REG[arg];
	PC++;

	return TIME_XOR;
}

f32 asm_cmp(arg) {
	ACC = (REG[arg] == ACC) ? 0 : (REG[arg] < ACC) ? 1 : 2;
	PC++;

	return TIME_CMP;
}


f32 asm_skp(arg) {
	if (ACC != 0)
		PC += arg;
	PC++;

	return TIME_SKP;
}

f32 asm_jmp(arg) {
	PC = FC.Flags[arg] - 1;

	return TIME_JMP;
}

#undef Command
#undef Argument