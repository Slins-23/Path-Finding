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

	if (this->costMode && node.cost > 1) {

		SDL_Color color = { 0, 0, 0, 255 };

		std::string str = std::to_string(node.cost);

		str = str[0];

		SDL_Surface* surface = TTF_RenderText_Solid(this->cost_font, str.c_str(), color);

		SDL_Texture* txt = SDL_CreateTextureFromSurface(this->renderer, surface);

		int w;
		int h;

		SDL_QueryTexture(txt, NULL, NULL, &w, &h);

		SDL_Rect txtRect;

		if (node.cost >= 10) {
			txtRect = { node.getX() + (w / 6), node.getY() + (h / 8), w, h };
		}
		else if (node.cost < 10) {
			txtRect = { node.getX() + (int) (w / 1.25), node.getY() + (h / 8), w, h };
		}

		SDL_RenderCopy(this->renderer, txt, NULL, &txtRect);

		SDL_DestroyTexture(txt);
		SDL_FreeSurface(surface);
	}
}

void Engine::updateGrid() {
	if (!this->loaded || this->reset || this->mode_changed) {
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
		this->mode_changed = false;
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


	if (this->costMode) {
		if (this->useAStar) {
			this->nodes.at(this->startIDX).fLocalGoal = 0.0f;
			this->nodes.at(this->startIDX).fGlobalGoal = heuristic(this->nodes.at(this->startIDX), this->nodes.at(this->targetIDX));
			this->AStarList.clear();
			this->AStarList.push_back(this->nodes.at(this->startIDX));
		}
		else if (!this->useAStar) {
			this->frontierPQ.clear();
			this->nodes.at(this->startIDX).cost_so_far = 0;
			this->frontierPQ.insert(std::make_pair(0, this->startIDX));
		}


	}
	else if (!this->costMode) {
		//this->nodes.at(this->startIDX).setType("visited");
		//this->nodes.at(this->startIDX).setColors("start");
		//this->frontier.push(this->nodes.at(this->startIDX));
		this->frontierGBFS.insert(std::make_pair(0, this->startIDX));
	}
	
	updateFrontier();
	setPath();
}

double Engine::heuristic(Node a, Node b) {
	//return abs(a.getX() - b.getX()) + abs((a.getY() - b.getY()));
	
	return sqrtf((a.getX() - b.getX()) * (a.getX() - b.getX()) + (a.getY() - b.getY()) * (a.getY() - b.getY()));
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

		double cost = this->frontierPQ.begin()->first;
		int nodeIDX = this->frontierPQ.begin()->second;
		this->frontierPQ.erase(this->frontierPQ.begin());


		if (nodeIDX == this->targetIDX) {
			this->targetFound = true;
			break;
		}

		this->nodes.at(nodeIDX).setType("visited");

		if (this->nodes.at(nodeIDX).getType() == "target") {
			this->nodes.at(nodeIDX).setColors("target");
		}
		else if (this->nodes.at(nodeIDX).getType() == "start") {
			this->nodes.at(nodeIDX).setColors("start");
		}

		for (Node neighbor : getNeighbors(this->nodes.at(nodeIDX))) {

			if (neighbor.getType() != "impassable") {

				int nodeN = neighbor.index;
				double costN = neighbor.cost;
				double distance = cost + costN;

				if (distance < neighbor.cost_so_far) {
					this->frontierPQ.erase(std::make_pair(neighbor.cost_so_far, nodeN));

					this->nodes.at(nodeN).cost_so_far = distance;
					this->nodes.at(nodeN).cameFrom = nodeIDX;

					this->frontierPQ.insert(std::make_pair(this->nodes.at(nodeN).cost_so_far, nodeN));
				}

				if (neighbor.getType() == "target") {
					this->nodes.at(neighbor.index).setType("visited");
					this->nodes.at(neighbor.index).setColors("target");
				}
				else if (neighbor.getType() == "start") {
					this->nodes.at(neighbor.index).setType("visited");
					this->nodes.at(neighbor.index).setColors("start");
				}
				else {
					this->nodes.at(neighbor.index).setType("visited");
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

	while (!this->AStarList.empty() && this->AStarList.front().index != this->targetIDX) {

		std::cout << "hrhrhr";

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

		//this->AStarList.sort([](const Node a, const Node b) { return a.fGlobalGoal < b.fGlobalGoal; });
		this->AStarList.sort([](const Node a, const Node b) { return a.fGlobalGoal < b.fGlobalGoal;  });

		if (!this->AStarList.empty() && this->nodes.at(this->AStarList.front().index).getType() == "visited") {
			this->AStarList.pop_front();
		}

		if (this->AStarList.empty()) {
			break;
		}

		Node currentNode = this->AStarList.front();

		if (this->nodes.at(currentNode.index).getType() == "impassable") {
			continue;
		}
		else {
			if (currentNode.index == this->startIDX) {
				this->nodes.at(currentNode.index).setType("visited");
				this->nodes.at(currentNode.index).setColors("start");
			}
			else if (currentNode.index == this->targetIDX) {
				this->nodes.at(currentNode.index).setType("visited");
				this->nodes.at(currentNode.index).setColors("target");
			}
			else {
				this->nodes.at(currentNode.index).setType("visited");
			}

		}


		for (Node neighbor : getNeighbors(this->nodes.at(currentNode.index))) {

			if (neighbor.getType() != "impassable" && neighbor.getType() != "visited") {
				this->AStarList.push_back(neighbor);
			}

			//float distance = this->nodes.at(currentNode.index).fLocalGoal + heuristic(this->nodes.at(currentNode.index), this->nodes.at(neighbor.index));
			//float distance = this->nodes.at(currentNode.index).fLocalGoal + heuristic(this->nodes.at(currentNode.index), this->nodes.at(neighbor.index));
			float distance = this->nodes.at(currentNode.index).fLocalGoal + this->nodes.at(neighbor.index).cost + this->nodes.at(currentNode.index).cost; //heuristic(this->nodes.at(currentNode.index), this->nodes.at(neighbor.index));
			//float distance = this->nodes.at(currentNode.index).cost_so_far + heuristic(this->nodes.at(currentNode.index), this->nodes.at(neighbor.index));

			
			// distance -> fPossiblyLowerGoal -> new_cost

			// cost_so_far -> fLocalGoal
			// priority -> fGlobalGoal

			if (distance < neighbor.fLocalGoal) {
				this->nodes.at(neighbor.index).cameFrom = currentNode.index;
				this->nodes.at(neighbor.index).fLocalGoal = distance;
				//this->nodes.at(neighbor.index).cost_so_far = distance;

				this->nodes.at(neighbor.index).fGlobalGoal = distance + heuristic(this->nodes.at(neighbor.index), this->nodes.at(this->targetIDX));

			}

			//clearWindow();
			//updateGrid();
			//updateRenderer();

		}
	}
}

void Engine::computePathBFS() {
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
}

void Engine::computePathGBFS() {
	std::vector<int> doneNodes;

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

		bool found = false;

		int currentNodeIDX = this->frontierGBFS.begin()->second;
		this->frontierGBFS.erase(this->frontierGBFS.begin());

		for (int idx : doneNodes) {
			if (idx == currentNodeIDX || idx == this->targetIDX) {
				found = true;
				break;
			}
		}

		

		if (!found) {
			for (Node neighbor : getNeighbors(this->nodes.at(currentNodeIDX))) {

				if (neighbor.getType() != "impassable" && neighbor.getType() != "visited") {

					if (neighbor.getType() == "target") {
						this->nodes.at(neighbor.index).setType("visited");
						this->nodes.at(neighbor.index).setColors("target");
					}
					else {
						this->nodes.at(neighbor.index).setType("visited");
					}

					this->nodes.at(neighbor.index).cameFrom = currentNodeIDX;
					this->frontierGBFS.insert(std::make_pair(heuristic(this->nodes.at(this->targetIDX), this->nodes.at(neighbor.index)), neighbor.index));
					clearWindow();
					updateGrid();
					updateRenderer();
				}
			}

			doneNodes.push_back(currentNodeIDX);
		}
	}
}

void Engine::updateFrontier() {

	if (this->costMode) {
		if (this->useAStar) {
			computePathAStar();
		}
		else if (!this->useAStar) {
			computePathDijkstra();
		}
		
		

		if ((this->frontierPQ.empty() && this->targetIDX != 12345) || this->targetFound) {
			resolvePath();
		}
	}
	else if (!this->costMode) {
		//computePathBFS();
		computePathGBFS();

		if (this->frontier.empty() && this->targetIDX != 12345) {
			resolvePath();
		}
	}
}

void Engine::resolvePath() {

	this->path = std::vector<int>();

	int target = this->targetIDX;

	Node currentNode = this->nodes.at(target);

	if (!this->costMode) {
		while (currentNode.index != this->nodes.at(this->startIDX).index) {
			this->path.push_back(currentNode.index);
			currentNode = this->nodes.at(currentNode.cameFrom);
		}

		this->path.push_back(this->startIDX);

		this->complete = true;
	}
	else if (this->costMode) {
		
		while (target != 0) {
			path.push_back(target);
			target = this->nodes.at(target).cameFrom;
		}

		this->complete = true;
	}

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
				if (this->costMode) {
					double cost = this->nodes.at(i).cost;
					this->nodes.at(i).setType("passable");
					this->nodes.at(i).cost = cost;
				}
				else {
					this->nodes.at(i).setType("passable");
				}

			}
			else if (isPath && this->startIDX == i) {
				this->nodes.at(this->startIDX).setType("start");
			}
			else if (isPath && this->targetIDX == i) {
				this->nodes.at(this->targetIDX).setType("target");
			}
		}
	}

	this->targetFound = false;
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

std::vector<Node> Engine::getNeighbors(Node node) {
	int nodeIdx = node.index;

	std::vector<Node> neighbors;

	if (nodeIdx > this->nodesPerRowIDX) {
		Node topNeighbor = this->nodes.at(nodeIdx - (this->nodesPerRowIDX + 1));
		neighbors.push_back(topNeighbor);
	}

	if (nodeIdx <= this->nodeCountIDX - this->nodesPerRowIDX - 1) {
		Node bottomNeighbor = this->nodes.at(nodeIdx + this->nodesPerRowIDX + 1);
		neighbors.push_back(bottomNeighbor);
	}
	
	if (nodeIdx % (this->nodesPerRowIDX + 1) != 0) {
		Node leftNeighbor = this->nodes.at(nodeIdx - 1);
		neighbors.push_back(leftNeighbor);
	}

	if (!(nodeIdx % (this->nodesPerRowIDX + 1) == 19)) {
		Node rightNeighbor = this->nodes.at(nodeIdx + 1);
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

int Engine::getCurrentNode() {
	return this->newIndex;
}