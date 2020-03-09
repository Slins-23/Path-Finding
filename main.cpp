#include "Engine.h"
#include <time.h>
#include "SDL_ttf.h"

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
					else if (event.button.button == SDL_BUTTON_RIGHT) {

						if (engine.costMode) {
							if (engine.nodes.at(engine.getCurrentNode()).getType() == "passable") {

								if (engine.nodes.at(engine.getCurrentNode()).cost < 4) {
									engine.nodes.at(engine.getCurrentNode()).cost += 1;
								}
								else if (engine.nodes.at(engine.getCurrentNode()).cost == 4) {
									engine.nodes.at(engine.getCurrentNode()).cost += 1;
									engine.nodes.at(engine.getCurrentNode()).setType("impassable");
								}

							}
						}
					}

					break;
				case SDL_MOUSEBUTTONUP:
					engine.isHeld = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_K:
						if (engine.complete) {
							engine.complete = false;
							
							for (int i = 1; i < engine.path.size() - 1; i++) {
								if (engine.costMode) {
									double cost = engine.nodes.at(engine.path.at(i)).cost;
									engine.nodes.at(engine.path.at(i)).setType("passable");
									engine.nodes.at(engine.path.at(i)).cost = cost;
								}
								else {
									engine.nodes.at(engine.path.at(i)).setType("passable");
								}

							}

							engine.nodes.at(engine.path.at(0)).setType("target");
							engine.nodes.at(engine.path.at(engine.path.size() - 1)).setType("start");

							if (engine.costMode) {
								engine.nodes.at(engine.path.at(0)).cost = 1;
								engine.nodes.at(engine.path.at(0)).cost_so_far = max_cost;

								engine.nodes.at(engine.path.at(engine.path.size() - 1)).cost = 1;
								engine.nodes.at(engine.path.at(engine.path.size() - 1)).cost_so_far = max_cost;
							}

							engine.reset = false;
							engine.playing = false;
							engine.paused = false;
							engine.targetFound = false;
						}

						
						break;
					case SDL_SCANCODE_P:
						std::cout << "Started" << std::endl;
						engine.setViewOnly(true);
						engine.playing = true;
						engine.paused = false;

						engine.play();

						engine.playing = false;
						engine.setViewOnly(false);

						engine.complete = true;
						break;
					case SDL_SCANCODE_R:
						std::cout << "Reset" << std::endl;
						engine.setViewOnly(false);
						engine.reset = true;
						engine.playing = false;
						engine.paused = false;
						engine.targetFound = false;
						engine.startIDX = 12345;
						engine.targetIDX = 12345;
						break;
					case SDL_SCANCODE_S:
						std::cout << "Select starting node." << std::endl;
						engine.setViewOnly(false);
						engine.pickStart = true;
						engine.pickTarget = false;
						break;
					case SDL_SCANCODE_T:
						std::cout << "Select target node." << std::endl;
						engine.setViewOnly(false);
						engine.pickTarget = true;
						engine.pickStart = false;
						break;
					case SDL_SCANCODE_V:
						engine.setViewOnly(!engine.viewOnly);
						break;
					case SDL_SCANCODE_C:
						engine.setCostMode(!engine.costMode);
						break;
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

									if (engine.costMode) {
										engine.nodes.at(engine.getCurrentNode()).cost = 1;
									}
								}
								else if (currentType == "passable") {
									engine.nodes.at(engine.getCurrentNode()).setType("impassable");

									if (engine.costMode) {
										engine.nodes.at(engine.getCurrentNode()).cost = 5;
									}
								}

								engine.lastNodeChange = engine.getCurrentNode();
							}
							else if (engine.pickStart && !engine.pickTarget) {
								engine.nodes.at(engine.getCurrentNode()).setType("start");
								engine.nodes.at(engine.getCurrentNode()).cost = 1;
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
								engine.nodes.at(engine.getCurrentNode()).cost = 1;
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