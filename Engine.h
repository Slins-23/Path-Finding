#include "Node.h"
#include <vector>
#include <queue>
#include <list>

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

	TTF_Font* cost_font;

	std::vector<Node> nodes;
	std::vector<int> path;

	bool reset = false;
	bool paused = false;
	bool playing = false;

	bool pickStart = false;
	bool pickTarget = false;

	bool viewOnly = false;
	bool costMode = false;

	bool isHeld = false;

	bool complete = false;

	bool mode_changed = false;

	bool targetFound = false;

	int startIDX = 12345;
	int targetIDX = 12345;

	int lastNodeChange = 12345;

	bool useAStar = true;

	std::queue<Node> frontier;

	std::set<std::pair<double, int>> frontierPQ;

	std::set<std::pair<int, int>> frontierGBFS;

	std::set<std::pair<int, int>> frontierAStar;

	std::list<Node> AStarList;

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
	double heuristic(Node a, Node b);
	void computePathBFS();
	void computePathGBFS();
	void computePathDijkstra();
	void computePathAStar();
	void updateFrontier();
	void resolvePath();
	void setPath();
	void setViewOnly(bool status);
	void setCostMode(bool costMode);
	std::vector<Node> getNeighbors(Node node);

	int getCurrentNode();
	int getWidth();
	int getHeight();

	const char* getTitle();
	SDL_Window& getWindow();
	SDL_Renderer& getRenderer();
	SDL_Event getEvent();
};