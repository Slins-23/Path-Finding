#include "Engine.h"
#include <time.h>

int main(int argc, char* argv[]) {
	Engine engine = Engine();

	SDL_Event event = engine.getEvent();

	srand(time(0));
	const int FPS = 60; // Framerate.
	const int frameDelay = 1000 / FPS;

	if (engine.load()) { // If engine.load returns true, start the program.

		Uint32 frameStart;
		int frameTime;

		while (true) {
			frameStart = SDL_GetTicks();

			while (SDL_PollEvent(&event)) {
				switch (event.type) {

				// Closes the program if SDL_QUIT event is fired.
				case SDL_QUIT: 
					engine.close();
					return 1;

				// Everytime the mouse is moved, update the mouse position and get the current Node index. Change Node color once mouse is over it to the current Node type.
				case SDL_MOUSEMOTION:
					engine.updateMousePosition();
					break;
				case SDL_MOUSEBUTTONDOWN:
					engine.handleMouse(true, event.button.button);
					break;
				case SDL_MOUSEBUTTONUP:
					engine.handleMouse(false, NULL);
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {

					// If in cost mode, toggle the path finding algorithm between A* and Dijkstra's.
					case SDL_SCANCODE_J: 
						engine.changeAlgorithm();
						break;

					case SDL_SCANCODE_K:
						engine.clearVisited();
						break;

					// Starts the path finding.
					case SDL_SCANCODE_P: 
						engine.start();
						break;

					// Resets all tiles.
					case SDL_SCANCODE_R:
						engine.reset();
						break;

					// Enter starting point selection mode
					case SDL_SCANCODE_S: 
						engine.toggleStartMode();
						break;

					// Enter target point selection mode.
					case SDL_SCANCODE_T: 
						engine.toggleTargetMode();
						break;

					// Toggles view-only mode.
					case SDL_SCANCODE_V:
						engine.toggleViewOnly();
						break;
					
					// Toggles cost mode.
					case SDL_SCANCODE_C: 
						engine.toggleCostMode();
						break;
					}
					break;
				}
			}

			engine.clearWindow(); // Clears the window.

			engine.handleMouseClick(); // If not on view-only mode, handle left mouse click if clicked/held.
			
			engine.updateGrid(); // Updates the grid. Redraws all Nodes or resets Node array and redraws them, if "R" was pressed.

			engine.updateRenderer(); // Updates the renderer.

			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime) {
				SDL_Delay(frameDelay - frameTime);
			}
		}
	}
	else {
	std::cout << "Could not load engine." << std::endl;
	}

	return 1;
}