#include "Node.h"
#include <vector>
#include <queue>
#include <list>

#pragma once
class Engine
{
private:
	const char* TITLE = "A* Path Finding Algorithm"; // Window title.
	int WIN_W = 800; // Window width.
	int WIN_H = 600; // Window hegiht.
	SDL_Window* window; // Window pointer.
	SDL_Renderer* renderer; // Renderer pointer.
	SDL_Surface* surface; // Surface pointer.
	SDL_Event event; // Event variable.
	
	bool loaded = false; // Whether the engine was loaded.
	int currentIndex = 12345; // Last hovered over Node's index.
	int newIndex = 0; // Currently hovered over Node's index.

public:

	TTF_Font* cost_font; // Font for the cost.

	std::vector<Node> nodes; // All nodes are stored in this array.
	std::vector<int> path; // The path indexes gets stored in this array once it's found.

	bool reset = false; // Whether the Nodes should be reset.
	bool paused = false; // Whether the path finding is paused.
	bool playing = false; // Whether the path finding is playing.

	bool pickStart = false; // If the current selection mode is starting point.
	bool pickTarget = false; // If the current selection mode is target point.

	bool viewOnly = false; // If current selection mode is view-only.
	bool costMode = false; // If current path finding mode is using costs or not.

	bool isHeld = false; // Whether the left mouse button is being held or not.

	bool complete = false; // Whether the path finding has been completed.

	bool mode_changed = false; // Whether there was a change in the path finding modes.

	bool targetFound = false; // Whether the target Node has been visited while trying to find a path.

	int startIDX = 12345; // Starting Node index.
	int targetIDX = 12345; // Target Node index.

	int lastNodeChange = 12345; // Which Node index has had a change of focus into the current Node.

	bool useAStar = true; // Whether the A-Star algorithm is being used or not.

	std::queue<Node> frontier; // Node frontier for Breadth-First Search.

	std::set<std::pair<double, int>> frontierPQ; // Node frontier for Dijkstra's algorithm.

	std::set<std::pair<int, int>> frontierGBFS; // Node frontier for Greedy Best-First Search.

	std::list<Node> AStarList; // Node frontier for A*.

	int nodesPerRowIDX = (int) (this->WIN_W / 40 - 1); // How many nodes per row there are. Always decremented -1, as this is used as an index.
	int nodesPerColIDX = (int) (this->WIN_H / 40 - 1); // How many nodes per column there are. Always decremented -1, as this is used as an index.

	int nodeCountIDX = (nodesPerRowIDX + 1) * (nodesPerColIDX + 1) - 1; // How many Nodes there are. Always decremented -1, as this is used as an index.

	Engine(); // Engine constructor with default values and no arguments.
	Engine(int width, int height); // Engine constructor with default title but passing the window's width and height as arguments.
	Engine(const char* title, int width, int height); // Engine constructor without default values, window's title, width, and height as arguments.
	
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
	double distance(Node a, Node b);
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