#pragma once

#include "Util.h"
#include "Types.h"
#include "Graphics.h"

// shortcuts
#define PC       FC.ProgramPointer
#define ROM      FC.Memory.Buffer
#define ACC      FC.Accumulator
#define PIN      FC.CurrentPin
#define REG      FC.Registers

// assembly in binary
#define	ASM_NOP      0b0000

#define	ASM_PIN      0b0001
#define	ASM_INP      0b0010
#define	ASM_OUT      0b0011

#define	ASM_LDI      0b0100
#define	ASM_LDA      0b0101
#define	ASM_STA      0b0110

#define	ASM_INC      0b1000
#define ASM_DEC      0b1001
#define ASM_AND      0b1010
#define ASM_XOR      0b1011
#define	ASM_CMP      0b1100

#define	ASM_SKP      0b1101
#define	ASM_FLG      0b1110
#define	ASM_JMP      0b1111

// time needed to process assembly
#define	TIME_NOP     0.00

#define	TIME_PIN     1.25
#define	TIME_INP     1.25
#define	TIME_OUT     1.25

#define	TIME_LDI     0.10
#define	TIME_LDA     0.20
#define	TIME_STA     0.20

#define	TIME_INC     0.10
#define	TIME_DEC     0.20
#define	TIME_AND     2.00
#define	TIME_XOR     2.00
#define	TIME_CMP     1.50

#define	TIME_SKP     3.00
#define	TIME_FLG     0.00
#define	TIME_JMP     3.75

// pins you can connect to by using pin
#define	PIN_MEMORY1  0
#define	PIN_MEMORY2  1
#define	PIN_KEYBOARD 2
#define	PIN_SCREEN   3

// predefining commands
f32 asm_pin(arg);
f32 asm_inp();
f32 asm_out();

f32 asm_ldi(arg);
f32 asm_lda(arg);
f32 asm_sta(arg);

f32 asm_inc();
f32 asm_dec();
f32 asm_and(arg);
f32 asm_xor(arg);
f32 asm_cmp(arg);

f32 asm_skp(arg);
f32 asm_jmp(arg);

// ALL DATA ABOUT FC
struct {

	// ROM
	struct {
		u8 Buffer[512];
		u8 Sends[6];
		u8 Path[256];
	} Memory;

	// RAM
	u8 Registers[16];

	// EXTRA REGISTRES
	u8 Accumulator;
	u8 CurrentPin;
	i32 ProgramPointer;

	// DISPLAY
	struct {
		u8 Sends[3];
		u8 SPointer;
	} Screen;

	// RUNTIME FLAGS
	i32 Flags[16];

	// KEYBOARD INPUT
	struct {
		bool Left;
		bool Right;
		bool Up;
		bool Down;
	} Key;

	// IS ON
	bool IsRunning;

} FC;

fn Quit(u8* message) {
	
	// shows error message to console
	printf(message);

	// saving rom
	FILE* fptr = fopen(FC.Memory.Path, "w");
	fwrite(ROM, 1, 512, fptr);
	fclose(fptr);

	// ending animation
	for (i32 i = 16; i > 0; i--) {
		for (u8 j = 0; j < 32; j++)
			SetPixel((u8)(rand() / 2048.0), (u8)(rand() / 2048.0));
		RenderUpdate();
		SDL_Delay(1000 / FRAMERATE);
	}

	// quiting the fc
	SDL_Delay(200);
	KillSDL();
	exit(0);
}