#include "Node.h"
#include <vector>
#include <queue>

#pragma once
class Engine
{
private:
	const char* TITLE = "A* Path Finding Algorithm";
	int WIN_W = 800;
	int WIN_H = 600;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Event event;
	
	bool loaded = false;
	int currentIndex = 12345;
	int newIndex = 0;

public:
	std::vector<Node> nodes;
	std::vector<int> path;

	bool reset = false;
	bool paused = false;
	bool playing = false;

	bool pickStart = false;
	bool pickTarget = false;

	bool viewOnly = false;

	bool isHeld = false;

	bool complete = false;

	int startIDX = 12345;
	int targetIDX = 12345;

	int lastNodeChange = 12345;

	std::queue<Node> frontier;

	int nodesPerRowIDX = (int) (this->WIN_W / 40 - 1);
	int nodesPerColIDX = (int) (this->WIN_H / 40 - 1);

	int nodeCountIDX = (nodesPerRowIDX + 1) * (nodesPerColIDX + 1) - 1;

	Engine();
	Engine(const char* title, int width, int height);
	Engine(int width, int height);
	~Engine();
	
	int load();
	void close();
	void clearWindow();
	void drawNode(Node node);
	void updateRenderer();
	void updateGrid();
	void handleNodes(int mouseX, int mouseY, int newX, int newY);
	void play();
	void updateFrontierBFS();
	void resolvePath();
	void setPath();
	void setViewOnly(bool status);
	std::vector<Node> getNeighbors(Node node);

	int getCurrentNode();
	int getWidth();
	int getHeight();

	const char* getTitle();
	SDL_Window& getWindow();
	SDL_Renderer* getRenderer();
	SDL_Event getEvent();
};