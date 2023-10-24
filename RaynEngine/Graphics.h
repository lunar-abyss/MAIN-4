// Graphics.h: provides functions and macros for graphics and window management. | Lunaryss © 2023
#pragma once

#include "Types.h"
#include <SDL.h>


// definitions
#define WINDOW_WIDTH            16
#define WINDOW_HEIGHT           16
#define WINDOW_SCALE            30
#define SCALED_WINDOW_WIDTH     WINDOW_WIDTH * WINDOW_SCALE
#define SCALED_WINDOW_HEIGHT    WINDOW_HEIGHT * WINDOW_SCALE
#define FRAMERATE               16

#define MAIN_COLOR     0x007dbc87
#define SCREEN_INERTIA 0.5


// initializes sdl
#define InitSDL() \
	SDL_Init(SDL_INIT_EVERYTHING); \
	Graphics.Window = SDL_CreateWindow("MAIN-4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCALED_WINDOW_WIDTH, SCALED_WINDOW_HEIGHT, null); \
	Graphics.Renderer = SDL_CreateRenderer(Graphics.Window, null, null); \
	Graphics.Texture = SDL_CreateTexture(Graphics.Renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT)

// destroys sdl
#define KillSDL() \
	SDL_DestroyTexture(Graphics.Texture); \
	SDL_DestroyRenderer(Graphics.Renderer); \
	SDL_DestroyWindow(Graphics.Window)

// updates the renderer
#define RenderUpdate() \
	SDL_SetRenderDrawColor(Graphics.Renderer, 0, 0, 0, 255); \
	SDL_RenderClear(Graphics.Renderer); \
	SDL_UpdateTexture(Graphics.Texture, null, Graphics.Buffer, WINDOW_WIDTH * 4); \
	SDL_RenderCopyEx(Graphics.Renderer, Graphics.Texture, null, null, 0.0, null, null); \
	SDL_RenderPresent(Graphics.Renderer)


// graphics structure
static struct {
	SDL_Window*   Window;
	SDL_Texture*  Texture;
	SDL_Renderer* Renderer;
	u32           Buffer[WINDOW_WIDTH * WINDOW_HEIGHT];
} Graphics;


// puts black pixel on screen
fn SetPixel(u8 x, u8 y) {
	Graphics.Buffer[x + y * WINDOW_WIDTH] = 0;
}

// interpolates color 1 to color 2
u32 LerpColor(u32 color1, u32 color2, f32 interpolation) {
	u8 color1rgb[3] = { (u8)(color1 >> 16), ((color1 & 65535) >> 8), (color1 & 255) };
	u8 color2rgb[3] = { (u8)(color2 >> 16), ((color2 & 65535) >> 8), (color2 & 255) };
	return
		((u32)((color2rgb[0] - color1rgb[0]) * interpolation + color1rgb[0]) << 16) +
		((u32)((color2rgb[1] - color1rgb[1]) * interpolation + color1rgb[1]) << 8) +
		 (u32)((color2rgb[2] - color1rgb[2]) * interpolation + color1rgb[2]);
}

// makes screen inertia
fn BrighterScreen() {
	for (i32 i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
		if (Graphics.Buffer[i] != MAIN_COLOR)
			Graphics.Buffer[i] = LerpColor(Graphics.Buffer[i], MAIN_COLOR, SCREEN_INERTIA);
}
