#include "Engine.h"

Engine::Engine() {
	this->window = NULL;
	this->renderer = NULL;
	this->cost_font = NULL;
	this->event = SDL_Event();

	std::cout << "Successfully initialized engine." << std::endl;
}

Engine::Engine(int width, int height) {
	this->WIN_W = width;
	this->WIN_H = height;
	this->window = NULL;
	this->renderer = NULL;
	this->cost_font = NULL;
	this->event = SDL_Event();

	std::cout << "Successfully initialized engine." << std::endl;
}

Engine::Engine(const char* title, int width, int height) {
	this->TITLE = title;
	this->WIN_W = width;
	this->WIN_H = height;
	this->window = NULL;
	this->renderer = NULL;
	this->cost_font = NULL;
	this->event = SDL_Event();

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

	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

	if (this->renderer == NULL) {
		std::cout << "Could not initialize SDL Renderer. Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(this->window);
		return 0;
	}

	if (TTF_Init() == -1) {
		std::cout << "Could not initialize fonts. Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(this->window);
		SDL_DestroyRenderer(this->renderer);
		return 0;
	}

	std::cout << "Successfully initialized Engine." << std::endl;

	return 1;
}

void Engine::close() {
	TTF_Quit();
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
}

void Engine::clearWindow() {
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);
}

void Engine::updateRenderer() {
	SDL_RenderPresent(this->renderer);
}

void Engine::drawNode(Node* node) {
	SDL_SetRenderDrawColor(this->renderer, node->getRed(), node->getGreen(), node->getBlue(), node->getAlpha());

	SDL_Rect rect;
	rect.x = node->getX();
	rect.y = node->getY();
	rect.w = node->getWidth();
	rect.h = node->getHeight();

	SDL_RenderFillRect(this->renderer, &rect);

	SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderDrawRect(this->renderer, &rect);

	if (this->costMode && node->cost > 1) {

		SDL_Color color = { 0, 0, 0, 255 };

		std::string str = std::to_string(node->cost);

		str = str[0];

		SDL_Surface* surface = TTF_RenderText_Solid(this->cost_font, str.c_str(), color);

		SDL_Texture* txt = SDL_CreateTextureFromSurface(this->renderer, surface);

		int w;
		int h;

		SDL_QueryTexture(txt, NULL, NULL, &w, &h);

		SDL_Rect txtRect;

		if (node->cost >= 10) {
			txtRect = { node->getX() + (w / 6), node->getY() + (h / 8), w, h };
		}
		else if (node->cost < 10) {
			txtRect = { node->getX() + (int)(w / 1.25), node->getY() + (h / 8), w, h };
		}

		SDL_RenderCopy(this->renderer, txt, NULL, &txtRect);

		SDL_DestroyTexture(txt);
		SDL_FreeSurface(surface);
	}
}

void Engine::updateGrid() {
	if (!this->loaded || this->reset || this->mode_changed) {
		this->nodes = std::vector<Node*>();

		int idx = 0;

		int storedIDX = 0;

		for (int y = 0; y < this->WIN_H; y += 40) {
			for (int x = 0; x < this->WIN_W; x += 40) {
				Node* node = new Node("passable", x, y);
				node->index = idx;

				this->nodes.push_back(node);
				this->nodes.back()->index = idx;
				idx++;
			}
		}

		this->loaded = true;
		this->reset = false;
		this->mode_changed = false;
	}

	for (Node* node : this->nodes) {
		drawNode(node);
	}
}

void Engine::handleNodes(int mouseX, int mouseY, int newX, int newY) {

	if ((newY / 40 * this->WIN_W / 40) + newX / 40 < this->nodes.size()) {
		this->currentNode = this->nodes.at((newY / 40 * this->WIN_W / 40) + newX / 40);
	}

	if (this->lastNode == nullptr && !this->reset && this->loaded) {
		this->lastNode = this->currentNode;
		this->lastNode->setColors("impassable");
	}
	else if (this->lastNode != nullptr && !this->reset) {

		if (this->lastNode != this->currentNode) {
			if (newX >= 0 && newY >= 0 && newX < this->WIN_W && newY < this->WIN_H) {

				if (!this->pickStart && !this->pickTarget) {

					if (this->currentNode->getType() == "passable") {
						this->currentNode->setColors("impassable");
					}
					else if (this->currentNode->getType() == "impassable") {
						this->currentNode->setColors("passable");
					}
					else {
						this->currentNode->setColors("passable");
					}
				}
				else if (this->pickStart && !this->pickTarget) {
					this->currentNode->setColors("start");
				}
				else if (!this->pickStart && this->pickTarget) {
					this->currentNode->setColors("target");
				}

				this->lastNode->setColors(this->lastNode->getType());
				this->lastNode = this->currentNode;
			}
		}
	}
	else if (this->reset) {
		this->reset = false;
		this->lastNode = nullptr;
	}

}

void Engine::start() {
	if (this->costMode) {
		if (this->useAStar) {
			this->startNode->fLocalGoal = 0.0f;
			this->startNode->fGlobalGoal = heuristic(this->startNode, this->targetNode);
			this->AStarList.clear();
			this->AStarList.push_back(this->startNode);
			computePathAStar();

			if ((this->frontierPQ.empty() && this->targetNode != nullptr) || this->targetFound) {
				resolvePath();
			}
		}
		else if (!this->useAStar) {
			this->frontierPQ.clear();
			this->startNode->cost_so_far = 0;
			this->frontierPQ.insert(std::make_pair(0, this->startNode));
			computePathDijkstra();

			if ((this->AStarList.empty() && this->targetNode != nullptr) || this->targetFound) {
				resolvePath();
			}
		}


	}
	else if (!this->costMode) {
		if (this->useGBFS) {
			this->frontierGBFS.insert(std::make_pair(0, this->startNode));
			computePathGBFS();

			if ((this->AStarList.empty() && this->targetNode != nullptr) || this->targetFound) {
				resolvePath();
			}
		}
		else if (!this->useGBFS) {
			this->startNode->setType("visited");
			this->startNode->setColors("start");
			this->frontier.push(this->startNode);
			computePathBFS();

			if ((this->AStarList.empty() && this->targetNode != nullptr) || this->targetFound) {
				resolvePath();
			}
		}


	}

	drawPath();
}

double Engine::heuristic(Node* a, Node* b) {
	return sqrtf((a->getX() - b->getX()) * (a->getX() - b->getX()) + (a->getY() - b->getY()) * (a->getY() - b->getY()));
}

void Engine::computePathDijkstra() {
	while (!this->frontierPQ.empty()) {

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

		float cost = this->frontierPQ.begin()->first;
		Node* currentNode = this->frontierPQ.begin()->second;
		this->frontierPQ.erase(this->frontierPQ.begin());


		if (currentNode == this->targetNode) {
			this->targetFound = true;
			break;
		}

		currentNode->setType("visited");

		if (currentNode->getType() == "target") {
			currentNode->setColors("target");
		}
		else if (currentNode->getType() == "start") {
			currentNode->setColors("start");
		}

		for (Node* neighbor : getNeighbors(currentNode)) {

			if (neighbor->getType() != "impassable") {

				double costN = neighbor->cost;
				double distance = cost + costN;

				if (distance < neighbor->cost_so_far) {
					this->frontierPQ.erase(std::make_pair(neighbor->cost_so_far, neighbor));

					neighbor->cost_so_far = distance;
					neighbor->cameFrom = currentNode;

					this->frontierPQ.insert(std::make_pair(neighbor->cost_so_far, neighbor));
				}

				if (neighbor->getType() == "target") {
					neighbor->setType("visited");
					neighbor->setColors("target");
				}
				else if (neighbor->getType() == "start") {
					neighbor->setType("visited");
					neighbor->setColors("start");
				}
				else {
					neighbor->setType("visited");
				}


				//clearWindow();
				//updateGrid();
				//updateRenderer();
			}
		}
	}
}

void Engine::computePathAStar() {
	//

	while (!this->AStarList.empty()) {

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

		this->AStarList.sort([](const Node* a, const Node* b) { return a->fGlobalGoal < b->fGlobalGoal;  });

		if (!this->AStarList.empty() && this->AStarList.front()->getType() == "visited") {
			this->AStarList.pop_front();
		}

		if (this->AStarList.empty()) {
			break;
		}

		Node* currentNode = this->AStarList.front();

		if (currentNode == this->targetNode) {
			this->targetFound = true;
			break;
		}

		if (currentNode->getType() == "impassable") {
			continue;
		}
		else {
			if (currentNode == this->startNode) {
				currentNode->setType("visited");
				currentNode->setColors("start");
			}
			else if (currentNode == this->targetNode) {
				currentNode->setType("visited");
				currentNode->setColors("target");
			}
			else {
				currentNode->setType("visited");
			}

		}


		for (Node* neighbor : getNeighbors(currentNode)) {


			if (neighbor->getType() != "impassable" && neighbor->getType() != "visited") {
				this->AStarList.push_back(neighbor);
			}

			//float distance = this->nodes.at(currentNode.index).fLocalGoal + heuristic(this->nodes.at(currentNode.index), this->nodes.at(neighbor.index));
			//float distance = this->nodes.at(currentNode.index).fLocalGoal + heuristic(this->nodes.at(currentNode.index), this->nodes.at(neighbor.index));
			float distance = currentNode->fLocalGoal + neighbor->cost;
			
			//float distance = this->nodes.at(currentNode.index).cost_so_far + heuristic(this->nodes.at(currentNode.index), this->nodes.at(neighbor.index));

			
			// distance -> fPossiblyLowerGoal -> new_cost

			// cost_so_far -> fLocalGoal
			// priority -> fGlobalGoal

			if (distance < neighbor->fLocalGoal) {
				neighbor->cameFrom = currentNode;
				neighbor->fLocalGoal = distance;
				//this->nodes.at(neighbor.index).cost_so_far = distance;

				neighbor->fGlobalGoal = distance + heuristic(neighbor, this->targetNode);
			}

			//clearWindow();
			//updateGrid();
			//updateRenderer();

		}

		if (currentNode == this->targetNode) {
			break;
		}
	}
}

void Engine::computePathBFS() {
	std::vector<Node*> doneNodes;

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
		bool shouldContinue = false;

		Node* currentNode = this->frontier.front();

		for (Node* node : doneNodes) {
			//if (node == currentNode || node == this->targetNode) {
			if (node == this->targetNode) {
				found = true;
				break;
			}
			else if (node == currentNode) {
				shouldContinue = true;
				break;
			}
		}

		if (shouldContinue) {
			continue;
		}

		if (!found) {
			for (Node* neighbor : getNeighbors(currentNode)) {

				if (neighbor->getType() != "impassable" && neighbor->getType() != "visited") {

					if (neighbor->getType() == "target") {
						neighbor->setType("visited");
						neighbor->setColors("target");
					}
					else {
						neighbor->setType("visited");
					}

					neighbor->cameFrom = currentNode;
					this->frontier.push(neighbor);
					clearWindow();
					updateGrid();
					updateRenderer();
				}
			}

			doneNodes.push_back(currentNode);
		}

		this->frontier.pop();
	}
}

void Engine::computePathGBFS() {
	std::vector<Node*> doneNodes;

	bool found = false;
	bool shouldContinue = false;

	while (!this->frontierGBFS.empty()) {

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

		Node* currentNode = this->frontierGBFS.begin()->second;
		this->frontierGBFS.erase(this->frontierGBFS.begin());

		for (Node* node : doneNodes) {
			//if (node == currentNode || node == this->targetNode) {
			if (node == this->targetNode) {
				found = true;
				break;
			}
			else if (node == currentNode) {
				shouldContinue = true;
				break;
			}

		}

		if (shouldContinue) {
			shouldContinue = false;
			continue;
		}

		if (!found) {
			for (Node* neighbor : getNeighbors(currentNode)) {

				if (neighbor->getType() != "impassable" && neighbor->getType() != "visited") {

					if (neighbor->getType() == "target") {
						neighbor->setType("visited");
						neighbor->setColors("target");
					}
					else {
						neighbor->setType("visited");
					}

					neighbor->cameFrom = currentNode;

					double h = heuristic(this->targetNode, neighbor);

					this->frontierGBFS.insert(std::make_pair(h, neighbor));
					clearWindow();
					updateGrid();
					updateRenderer();
				}
			}

			doneNodes.push_back(currentNode);
		}
	
	}
}

void Engine::resolvePath() {

	this->path = std::vector<Node*>();

	Node* currentNode = this->targetNode;

	while (currentNode != this->startNode) {
		this->path.push_back(currentNode);
		currentNode = currentNode->cameFrom;
	}

	this->path.push_back(this->startNode);
	this->complete = true;
}

void Engine::drawPath() {
	for (Node* node : this->nodes) {
		if (node->getType() == "visited") {
			bool isPath = false;

			for (Node* pathNode : this->path) {
				if (node == pathNode) {
					isPath = true;
					break;
				}
			}

			if (!isPath) {
				if (this->costMode) {
					float cost = node->cost;
					node->setType("passable");
					node->cost = cost;
				}
				else if (!this->costMode) {
					node->setType("passable");
				}
			}
			else if (isPath && node == this->startNode) {
				this->startNode->setType("start");
			}
			else if (isPath && node == this->targetNode) {
				this->targetNode->setType("target");
			}
		}
	}

	this->targetFound = false;
}

void Engine::setViewOnly(bool status) {
	if (status) {
		this->viewOnly = true;
		this->lastNode->setType(this->lastNode->getType());
	}
	else {
		this->viewOnly = false;
	}
}

void Engine::setCostMode(bool costMode) {
	this->mode_changed = true;

	if (costMode) {
		this->cost_font = TTF_OpenFont("fonts/Lato-Regular.ttf", 25);
		this->costMode = true;
	}
	else if (!costMode) {
		TTF_CloseFont(this->cost_font);
		this->costMode = false;
	}
}

std::vector<Node*> Engine::getNeighbors(Node* node) {
	std::vector<Node*> neighbors;

	if (node->index > this->nodesPerRowIDX) {
		Node* topNeighbor = this->nodes.at(node->index - (this->nodesPerRowIDX + 1));
		neighbors.push_back(topNeighbor);
	}

	if (node->index <= this->nodeCountIDX - this->nodesPerRowIDX - 1) {
		Node* bottomNeighbor = this->nodes.at(node->index + this->nodesPerRowIDX + 1);
		neighbors.push_back(bottomNeighbor);
	}
	
	if (node->index % (this->nodesPerRowIDX + 1) != 0) {
		Node* leftNeighbor = this->nodes.at(node->index - 1);
		neighbors.push_back(leftNeighbor);
	}

	if (!(node->index % (this->nodesPerRowIDX + 1) == 19)) {
		Node* rightNeighbor = this->nodes.at(node->index + 1);
		neighbors.push_back(rightNeighbor);
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

SDL_Renderer& Engine::getRenderer() {
	return *this->renderer;
}

SDL_Event Engine::getEvent() {
	return this->event;
}

Node* Engine::getCurrentNode() {
	return this->currentNode;
}