#include "Engine.h"

Engine::Engine() {
	this->window = NULL;
	this->renderer = NULL;

	std::cout << "Successfully initialized engine." << std::endl;
}

Engine::Engine(int width, int height) {
	this->WIN_W = width;
	this->WIN_H = height;
	this->window = NULL;
	this->renderer = NULL;

	std::cout << "Successfully initialized engine." << std::endl;
}

Engine::Engine(const char* title, int width, int height) {
	this->TITLE = title;
	this->WIN_W = width;
	this->WIN_H = height;
	this->window = NULL;
	this->renderer = NULL;

	std::cout << "Successfully initialized engine." << std::endl;
}

Engine::~Engine() {
	std::cout << "Successfully closed engine." << std::endl;
}

int Engine::load() {
	if (SDL_Init(SDL_INIT_VIDEO) != -1) {
		std::cout << "Successfully initialized SDL window." << std::endl;
	}
	else {
		std::cout << "Could not initialize SDL Video. Error: " << SDL_GetError() << std::endl;
		return 0;
	}

	this->window = SDL_CreateWindow(this->TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->WIN_W, this->WIN_H, SDL_WINDOW_SHOWN);

	if (this->window == NULL) {
		std::cout << "Could not initialize SDL Window. Error: " << SDL_GetError() << std::endl;
		return 0;
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (this->renderer == NULL) {
		std::cout << "Could not initialize SDL Renderer. Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		return 0;
	}

	std::cout << "Successfully initialized SDL." << std::endl;

	return 1;
}

void Engine::close() {
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void Engine::clearWindow() {
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);
}

void Engine::updateRenderer() {
	SDL_RenderPresent(this->renderer);
}

void Engine::drawNode(Node node) {
	SDL_SetRenderDrawColor(this->renderer, node.getRed(), node.getGreen(), node.getBlue(), node.getAlpha());

	SDL_Rect rect;
	rect.x = node.getX();
	rect.y = node.getY();
	rect.w = node.getWidth();
	rect.h = node.getHeight();

	SDL_RenderFillRect(this->renderer, &rect);

	SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderDrawRect(this->renderer, &rect);
}

void Engine::updateGrid() {
	if (!this->loaded || this->reset) {
		this->nodes = std::vector<Node>();

		int idx = 0;

		for (int y = 0; y < this->WIN_H; y += 40) {
			for (int x = 0; x < this->WIN_W; x += 40) {
				Node node = Node("passable", x, y);
				node.index = idx;

				this->nodes.push_back(node);
				idx++;
			}
		}

		this->loaded = true;
		this->reset = false;
	}

	for (Node node : this->nodes) {
		drawNode(node);
	}
}

void Engine::handleNodes(int mouseX, int mouseY, int newX, int newY) {
	this->newIndex = (newY / 40 * this->WIN_W / 40) + newX / 40;

	if (this->currentIndex == 12345 && !this->reset && this->loaded) {
		this->currentIndex = this->newIndex;

		this->nodes.at(currentIndex).setColors("impassable");
	}
	else if (this->currentIndex != 12345 && !this->reset) {

		if (this->currentIndex != this->newIndex) {
			if (newX >= 0 && newY >= 0 && newX < this->WIN_W && newY < this->WIN_H) {

				Node currentNode = this->nodes.at(this->currentIndex);
				Node newNode = this->nodes.at(this->newIndex);
				const char* currentType = currentNode.getType();
				const char* newType = newNode.getType();

				if (!this->pickStart && !this->pickTarget) {

					if (newType == "passable") {
						this->nodes.at(this->newIndex).setColors("impassable");
					}
					else if (newType == "impassable") {
						this->nodes.at(this->newIndex).setColors("passable");
					}
					else {
						this->nodes.at(this->newIndex).setColors("passable");
					}

					this->nodes.at(this->currentIndex).setColors(currentNode.getType());
				}
				else if (this->pickStart && !this->pickTarget) {
					this->nodes.at(this->newIndex).setColors("start");

					this->nodes.at(this->currentIndex).setColors(currentNode.getType());
				}
				else if (!this->pickStart && this->pickTarget) {
					this->nodes.at(this->newIndex).setColors("target");

					this->nodes.at(this->currentIndex).setColors(currentNode.getType());
				}

				this->currentIndex = this->newIndex;
			}
		}
	}
	else if (this->reset) {
		this->reset = false;
		this->currentIndex = 12345;
	}

}

void Engine::play() {
	this->nodes.at(this->startIDX).setType("visited");
	this->nodes.at(this->startIDX).setColors("start");
	this->frontier.push(this->nodes.at(this->startIDX));
	updateFrontierBFS();
	setPath();
}

void Engine::updateFrontierBFS() {
	std::vector<int> doneNodes;

	while (!this->frontier.empty()) {

		if (this->paused) {
			break;
		}

		while (SDL_PollEvent(&this->event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_P:
						this->paused = true;
						this->playing = false;
						break;
					}
				break;
			}

		}

		bool found = false;

		Node currentNode = this->frontier.front();

		for (int idx : doneNodes) {
			if (idx == currentNode.index || idx == this->targetIDX) {
				found = true;
				break;
			}
		}

		if (!found) {
			for (Node neighbor : getNeighbors(currentNode)) {

				if (neighbor.getType() != "impassable" && neighbor.getType() != "visited") {

					if (neighbor.getType() == "target") {
						this->nodes.at(neighbor.index).setType("visited");
						this->nodes.at(neighbor.index).setColors("target");
					}
					else {
						this->nodes.at(neighbor.index).setType("visited");
					}
					
					this->nodes.at(neighbor.index).cameFrom = currentNode.index;
					this->frontier.push(this->nodes.at(neighbor.index));
					clearWindow();
					updateGrid();
					updateRenderer();
				}
			}

			doneNodes.push_back(currentNode.index);
		}

		this->frontier.pop();
	}

	if (this->frontier.empty() && this->targetIDX != 12345) {
		resolvePath();
	}
}

void Engine::resolvePath() {
	this->path = std::vector<int>();

	Node currentNode = this->nodes.at(this->targetIDX);

	while (currentNode.index != this->nodes.at(this->startIDX).index) {
		this->path.push_back(currentNode.index);
		currentNode = this->nodes.at(currentNode.cameFrom);
	}

	this->path.push_back(this->startIDX);	

	this->complete = true;
}

void Engine::setPath() {
	for (int i = 0; i < this->nodes.size(); i++) {
		
		if (this->nodes.at(i).getType() == "visited") {

			bool isPath = false;

			for (int j = 0; j < this->path.size(); j++) {
				if (i == this->path.at(j)) {
					isPath = true;
					break;
				}
			}

			if (!isPath) {
				this->nodes.at(i).setType("passable");
			}
		}
	}

	std::cout << "HRRRR";
}

void Engine::setViewOnly(bool status) {
	if (status) {
		this->viewOnly = true;
		this->nodes.at(this->currentIndex).setType(this->nodes.at(currentIndex).getType());
	}
	else {
		this->viewOnly = false;
	}
}

std::vector<Node> Engine::getNeighbors(Node node) {
	int nodeIdx = node.index;

	std::vector<Node> neighbors;

	std::cout << "Current Index: " << nodeIdx << std::endl;

	if (nodeIdx > this->nodesPerRowIDX) {
		Node topNeighbor = this->nodes.at(nodeIdx - (this->nodesPerRowIDX + 1));
		neighbors.push_back(topNeighbor);
		std::cout << "Top Neighbor Index: " << topNeighbor.index << std::endl;
	}

	if (nodeIdx <= this->nodeCountIDX - this->nodesPerRowIDX - 1) {
		Node bottomNeighbor = this->nodes.at(nodeIdx + this->nodesPerRowIDX + 1);
		neighbors.push_back(bottomNeighbor);
		std::cout << "Bottom Neighbor Index: " << bottomNeighbor.index << std::endl;
	}
	
	if (nodeIdx % (this->nodesPerRowIDX + 1) != 0) {
		Node leftNeighbor = this->nodes.at(nodeIdx - 1);
		neighbors.push_back(leftNeighbor);
		std::cout << "Left Neighbor Index: " << leftNeighbor.index << std::endl;
	}

	if (!(nodeIdx % (this->nodesPerRowIDX + 1) == 19)) {
		Node rightNeighbor = this->nodes.at(nodeIdx + 1);
		neighbors.push_back(rightNeighbor);
		std::cout << "Right Neighbor Index: " << rightNeighbor.index << std::endl;
	}

	return neighbors;
}

int Engine::getWidth() {
	return this->WIN_W;
}

int Engine::getHeight() {
	return this->WIN_H;
}

const char* Engine::getTitle() {
	return this->TITLE;
}

SDL_Window& Engine::getWindow() {
	return *this->window;
}

SDL_Renderer* Engine::getRenderer() {
	return this->renderer;
}

SDL_Event Engine::getEvent() {
	return this->event;
}

int Engine::getCurrentNode() {
	return this->newIndex;
}