#include "FC.h"
#include "Util.h"
#include "Interpreter.h"

i32 main() {

	// running variable
	FC.IsRunning = true;

	// loading all data
	u8* path = concat(_getcwd(nova, null), "\\ROM.b");
	u8* buffer = FileReadBytes(path, 512);

	for (i32 i = 0; i < 512; i++) {
		if (buffer[i] < 16)
			ROM[i] = buffer[i];
		else
			Quit("ROM is corrupted.");
	}

	for (i32 i = 0; i < strlen(path); i++)
		FC.Memory.Path[i] = path[i];

	free(buffer); free(path);

	// initialising graphics
	InitSDL();
	for (i32 i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
		Graphics.Buffer[i] = MAIN_COLOR;

	InitInterpreter();

	// main loop
	while (FC.IsRunning) {

		// event
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				FC.IsRunning = null;
			else if (event.type == SDL_KEYDOWN) {
				if      (event.key.keysym.sym == SDLK_LEFT)  FC.Key.Left  = true;
				else if (event.key.keysym.sym == SDLK_RIGHT) FC.Key.Right = true;
				else if (event.key.keysym.sym == SDLK_UP)    FC.Key.Up    = true;
				else if (event.key.keysym.sym == SDLK_DOWN)  FC.Key.Down  = true;
			}
			else if (event.type == SDL_KEYUP) {
				if      (event.key.keysym.sym == SDLK_LEFT)  FC.Key.Left  = null;
				else if (event.key.keysym.sym == SDLK_RIGHT) FC.Key.Right = null;
				else if (event.key.keysym.sym == SDLK_UP)    FC.Key.Up    = null;
				else if (event.key.keysym.sym == SDLK_DOWN)  FC.Key.Down  = null;
			}
		}

		// interpreter
		BrighterScreen();
		Interpreter();

		// updating the renderer
		RenderUpdate();

		// fps
		SDL_Delay(1000 / FRAMERATE);
	}

	// stopping the game
	Quit("Strangely ended.");
}