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

	const char* getTitle(); // Returns the window title.
	int getWidth(); // Returns the window width.
	int getHeight(); // Returns the window height.

	void close(); // Closes the environment.
	void clearWindow(); // Clears the window.
	void drawNode(Node node); // Draws the Node passed as an argument.
	void updateRenderer(); // Updates the renderer.
	void updateGrid(); // Updates the grid.
	void handleNodes(int mouseX, int mouseY, int newX, int newY); // Handles the Node in the position passed as an argument and previous one, if necessary.
	void play(); // Runs the algorithm.

	void computePathBFS(); // Breadth-First Search algorithm.
	void computePathGBFS(); // Greedy Best-First Search algorithm.
	void computePathDijkstra(); // Dijkstra's algorithm.
	void computePathAStar(); // A* algorithm.
	void updateFrontier(); // Handles the selected algorithm and calls fucntion to update the path once the algorithm is finished.
	void resolvePath(); // Finds the path, starting from ther target Node.
	void drawPath(); // Draws the path found from the above function.
	void setViewOnly(bool status); // Sets view-only to the argument.
	void setCostMode(bool costMode); // Sets cost-mode to the argument.
	
	int load(); // Sets up the environment.
	int getCurrentNode(); // Returns the current hovered over Node's index.
	
	double heuristic(Node a, Node b); // Heuristic function.
	
	std::vector<Node> getNeighbors(Node node); // Finds all AXIS neighbors for the given Node.

	

	SDL_Window& getWindow(); // Returns reference to the window.
	SDL_Renderer& getRenderer(); // Returns reference to the renderer.
	SDL_Event getEvent(); // Returns the event.
};