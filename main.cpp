#include "Engine.h"
#include <time.h>

int main(int argc, char* argv[]) {
	Engine engine = Engine();

	SDL_Event event = engine.getEvent();

	int mouseX;
	int mouseY;

	int xPos;
	int yPos;

	int newX = 0;
	int newY = 0;

	srand(time(0));
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	if (engine.load()) {

		Uint32 frameStart;
		int frameTime;

		while (true) {
			frameStart = SDL_GetTicks();

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					engine.close();
					std::cout << "Successfully closed SDL." << std::endl;
					return 1;
				case SDL_MOUSEMOTION:
					if (!engine.reset && !engine.viewOnly) {
						SDL_GetGlobalMouseState(&mouseX, &mouseY);
						SDL_GetWindowPosition(&engine.getWindow(), &xPos, &yPos);

						newX = mouseX - xPos - 1;
						newY = mouseY - yPos - 1;

						engine.handleNodes(mouseX, mouseY, newX, newY);
					}	

					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						engine.isHeld = true;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					engine.isHeld = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_P:
						std::cout << "Started" << std::endl;
						engine.setViewOnly(true);
						engine.playing = true;
						engine.paused = false;

						engine.play();
						break;
					case SDL_SCANCODE_R:
						std::cout << "Reset" << std::endl;
						engine.setViewOnly(false);
						engine.reset = true;
						engine.playing = false;
						engine.paused = false;
						engine.startIDX = 12345;
						engine.targetIDX = 12345;
						break;
					case SDL_SCANCODE_S:
						std::cout << "Picked starting node." << std::endl;
						engine.setViewOnly(false);
						engine.pickStart = true;
						engine.pickTarget = false;
						break;
					case SDL_SCANCODE_T:
						std::cout << "Picked target node." << std::endl;
						engine.setViewOnly(false);
						engine.pickTarget = true;
						engine.pickStart = false;
						break;
					case SDL_SCANCODE_V:
						engine.setViewOnly(!engine.viewOnly);
					}
					break;
				}
			}
				
				engine.clearWindow();

				if (!engine.viewOnly) {

					if (engine.isHeld) {

						if (engine.getCurrentNode() != engine.lastNodeChange && engine.getCurrentNode() <= 300) {

							Node currentNode = engine.nodes.at(engine.getCurrentNode());
							const char* currentType = currentNode.getType();

							if (!engine.pickStart && !engine.pickTarget) {
								if (currentType == "impassable" || currentType == "start" || currentType == "target") {
									engine.nodes.at(engine.getCurrentNode()).setType("passable");
								}
								else if (currentType == "passable") {
									engine.nodes.at(engine.getCurrentNode()).setType("impassable");
								}

								engine.lastNodeChange = engine.getCurrentNode();
							}
							else if (engine.pickStart && !engine.pickTarget) {
								engine.nodes.at(engine.getCurrentNode()).setType("start");
								engine.pickStart = false;

								if (engine.startIDX != 12345) {
									engine.nodes.at(engine.startIDX).setType("passable");
									engine.startIDX = engine.getCurrentNode();
								}
								else {
									engine.startIDX = engine.getCurrentNode();
								}

								engine.lastNodeChange = engine.getCurrentNode();
							}
							else if (!engine.pickStart && engine.pickTarget) {
								engine.nodes.at(engine.getCurrentNode()).setType("target");
								engine.pickTarget = false;

								if (engine.targetIDX != 12345) {
									engine.nodes.at(engine.targetIDX).setType("passable");
									engine.targetIDX = engine.getCurrentNode();
								}
								else {
									engine.targetIDX = engine.getCurrentNode();
								}

								engine.lastNodeChange = engine.getCurrentNode();
							}
						}
					}
				}
			

				engine.updateGrid();

				engine.updateRenderer();

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