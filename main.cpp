#include "Engine.h"
#include <time.h>

int main(int argc, char* argv[]) {
	Engine engine = Engine();

	SDL_Event event = engine.getEvent();

	int mouseX; // Where the mouse X position gets stored. Updates everytime the mouse is moved. In the range of your display width.
	int mouseY; // Where the mouse Y position gets stored. Updates everytime the mouse is moved. In the range of your display height.

	int xPos;  // Where the window X position gets stored. Updates everytime the mouse is moved. In the range of your display width.
	int yPos;  // Where the window X position gets stored. Updates everytime the mouse is moved. In the range of your display width.

	// Decrements the window position from the display mouse position in order to get the below values.
	int newX = 0; // Where the mouse X position relative to the window's position. Updates everytime the mouse is moved. In the range of the window width.
	int newY = 0; // Where the mouse Y position relative to the window's position. Updates everytime the mouse is moved. In the range of the window height.

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
					std::cout << "Successfully closed SDL." << std::endl;
					return 1;

				// Everytime the mouse is moved, update the mouse position and get the current Node index. Change Node color once mouse is over it to the current Node type.
				case SDL_MOUSEMOTION:
					if (!engine.reset && !engine.viewOnly) {
						SDL_GetGlobalMouseState(&mouseX, &mouseY); // Updates mouseX and mouseY variables with the mouse position within the display.
						SDL_GetWindowPosition(&engine.getWindow(), &xPos, &yPos); // Updates xPos and yPos variables with the window position within the display.

						newX = mouseX - xPos - 1; // Updates newX with the mouse X position within the window. (With 0 being the leftmost points of the window).
						newY = mouseY - yPos - 1; // Updates newY with the mouse Y position within the window. (With 0 being the topmost points of the window).

						engine.handleNodes(mouseX, mouseY, newX, newY); // Handles node colors and states when they're hovered over.
					}	

					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						engine.isHeld = true; // Reference variable for checking if the left mouse button is being held.
					}
					else if (event.button.button == SDL_BUTTON_RIGHT) { // If right mouse button is clicked when hovering a Node inside "cost mode", increase the Node's cost by 1. If it reaches 5, becomes "impassable".

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
					engine.isHeld = false; // Reference variable for checking if the left mouse button is being held.
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {

					// If in cost mode, toggle the path finding algorithm between A* and Dijkstra's.
					case SDL_SCANCODE_J: 
						if (engine.costMode) {
							engine.useAStar = !engine.useAStar;
							std::cout << "Current algorithm: " << (engine.useAStar ? "A*" : "Dijkstra's") << std::endl;
						}
						else if (!engine.costMode) {
							engine.useGBFS = !engine.useGBFS;
							std::cout << "Current algorithm: " << (engine.useGBFS ? "A*" : "Dijkstra's") << std::endl;
						}
						break;

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
								engine.nodes.at(engine.path.at(0)).cost_so_far = INFINITY;

								engine.nodes.at(engine.path.at(engine.path.size() - 1)).cost = 1;
								engine.nodes.at(engine.path.at(engine.path.size() - 1)).cost_so_far = INFINITY;
							}

							engine.reset = false;
							engine.playing = false;
							engine.paused = false;
							engine.targetFound = false;
						}
						break;

					// Starts the path finding.
					case SDL_SCANCODE_P: 
						std::cout << "Started" << std::endl;
						engine.setViewOnly(true);
						engine.playing = true;
						engine.paused = false;

						engine.start();

						engine.playing = false;
						engine.setViewOnly(false);

						engine.complete = true;
						break;

					// Resets all tiles.
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

					// Enter starting point selection mode
					case SDL_SCANCODE_S: 
						std::cout << "Select starting node." << std::endl;
						engine.setViewOnly(false);
						engine.pickStart = true;
						engine.pickTarget = false;
						break;

					// Enter target point selection mode.
					case SDL_SCANCODE_T: 
						std::cout << "Select target node." << std::endl;
						engine.setViewOnly(false);
						engine.pickTarget = true;
						engine.pickStart = false;
						break;

					// Toggles view-only mode.
					case SDL_SCANCODE_V:
						engine.setViewOnly(!engine.viewOnly);
						break;
					
					// Toggles cost mode.
					case SDL_SCANCODE_C: 
						engine.setCostMode(!engine.costMode);
						break;
					}
					break;
				}
			}
				
				engine.clearWindow(); // Clears the window.

				// If not on view-only mode, handle left mouse click, if clicked/held.
				if (!engine.viewOnly) {

					if (engine.isHeld) {

						// Checks if the Node currently being hovered over has changed since the last time it was updated (implementation of "painting" Nodes) and if the Node's index does not exceed the Nodes array size.
						if (engine.getCurrentNode() != engine.lastNodeChange && engine.getCurrentNode() < engine.nodes.size()) {

							Node currentNode = engine.nodes.at(engine.getCurrentNode());
							const char* currentType = currentNode.getType();

							// If the active selection mode is not both start and target points.
							if (!engine.pickStart && !engine.pickTarget) { 


								// If Node is not passable, make it passable and update its cost to 1.
								if (currentType != "passable") {
									engine.nodes.at(engine.getCurrentNode()).setType("passable");

									if (engine.costMode) {
										engine.nodes.at(engine.getCurrentNode()).cost = 1;
									}
								}

								// If Node is passable, make it impassable and update its cost to 5.
								else if (currentType == "passable") {
									engine.nodes.at(engine.getCurrentNode()).setType("impassable");

									if (engine.costMode) {
										engine.nodes.at(engine.getCurrentNode()).cost = 5;
									}
								}

								// Updates the last changed node to be the current one.
								engine.lastNodeChange = engine.getCurrentNode();
							}

							// If the active selection mode is start but not target point.
							else if (engine.pickStart && !engine.pickTarget) {

								// Updates the Node to become the starting Node.
								engine.nodes.at(engine.getCurrentNode()).setType("start");
								engine.nodes.at(engine.getCurrentNode()).cost = 1;
								engine.pickStart = false;

								// If starting Node has already been set before, set the previous starting Node to passable and update the starting Node index to be the current Node's
								if (engine.startIDX != 12345) {
									engine.nodes.at(engine.startIDX).setType("passable");
									engine.startIDX = engine.getCurrentNode();
								}

								// If starting Node has not been set before, update the starting Node index to be the current Node's
								else {
									engine.startIDX = engine.getCurrentNode();
								}

								// Updates the last changed node to be the current one.
								engine.lastNodeChange = engine.getCurrentNode();
							}

							// If the active selection mode is target but not target start.
							else if (!engine.pickStart && engine.pickTarget) {

								// Updates the Node to become the target Node.
								engine.nodes.at(engine.getCurrentNode()).setType("target");
								engine.nodes.at(engine.getCurrentNode()).cost = 1;
								engine.pickTarget = false;

								// If target Node has already been set before, set the previous target Node to passable and update the target Node index to be the current Node's
								if (engine.targetIDX != 12345) {
									engine.nodes.at(engine.targetIDX).setType("passable");
									engine.targetIDX = engine.getCurrentNode();
								}

								// If target Node has not been set before, update the target Node index to be the current Node's
								else {
									engine.targetIDX = engine.getCurrentNode();
								}

								// Updates the last changed node to be the current one.
								engine.lastNodeChange = engine.getCurrentNode();
							}
						}
					}
				}
			
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