#include "Node.h"
#include <vector>
#include <queue>
#include <list>

#pragma once
class Engine
{
private:
	// Environment
	void updateCost(); // Updates the cost for the current Node.

	// SDL
	SDL_Window* window; // Window pointer.
	SDL_Renderer* renderer; // Renderer pointer.
	SDL_Event event; // Event variable.
	void handleNodes(int newX, int newY); // Handles the Node in the given position, and a previous one if necessary.
	void drawPath(); // Draws the path found from the above function.
	void drawNode(Node* node); // Draws the Node passed as an argument.

	// Fonts
	TTF_Font* cost_font; // Font for the cost.

	// Nodes
	Node* last_node_changed = nullptr; // Last Node refered to when the mouse focus changes to another Node.
	Node* start_node = nullptr; // Node set as the starting point.
	Node* target_node = nullptr; // Node set as the target.
	Node* last_node = nullptr; // Last hovered over Node's index.
	Node* current_node = nullptr; // Currently hovered over Node's index.

	// Arrays
	std::vector<Node*> nodes; // All nodes are stored in this array.
	std::vector<Node*> path; // The path indexes gets stored in this array once it's found.
	std::vector<Node*> visited_nodes; // All nodes that were visited by the algorihtm.

	// Priority Queues
	std::queue<Node*> frontier; // Nodes queue for Breadth-First Search.
	std::set<std::pair<float, Node*>> frontier_gbfs; // Nodes priority queue for Greedy Best-First Search.
	std::set<std::pair<float, Node*>> frontier_dijk; // Nodes priority queue for Dijkstra's algorithm.
	std::list<Node*> open; // List of open Nodes for the A-Star algorithm.

	// Algorithms
	void computePathBFS(); // Breadth-First Search algorithm.
	void computePathGBFS(); // Greedy Best-First Search algorithm.
	void computePathDijkstra(); // Dijkstra's algorithm.
	void computePathAStar(); // A* algorithm.
	void resolvePath(); // Finds the path, starting from ther target Node.	
	void drawGrid();
	void resetGrid();
	float heuristic(Node* a, Node* b); // Heuristic function.


	// Getters
	std::vector<Node*> getNeighbors(Node* node); // Finds all AXIS neighbors for the given Node.

	// Booleans
	bool loaded = false; // Whether the engine was loaded.
	bool paused = false; // Whether the path finding is paused.
	bool playing = false; // Whether the path finding is playing.
	bool pick_start = false; // If the current selection mode is starting point.
	bool pick_target = false; // If the current selection mode is target point.
	bool view_only = false; // If current selection mode is view-only.
	bool cost_mode = false; // If current path finding mode is using costs or not.
	bool is_held = false; // Whether the left mouse button is being held or not.
	bool complete = false; // Whether the path finding has been completed.
	bool mode_changed = false; // Whether there was a change in the path finding modes.
	bool target_found = false; // Whether the target Node has been visited while trying to find a path.
	bool use_AStar = true; // Whether the A-Star algorithm is being used or not.
	bool use_GBFS = true; // Whether the Greedy Best-First Search algorithm is being used or not.

	// Integers
	int WIN_W = 800; // Window width.
	int WIN_H = 600; // Window hegiht.
	int nodes_per_row_IDX = (int)(this->WIN_W / 40 - 1); // How many nodes per row there are. Always decremented -1, as this is used as an index.
	int nodes_per_col_IDX = (int)(this->WIN_H / 40 - 1); // How many nodes per column there are. Always decremented -1, as this is used as an index.
	int node_count_IDX = (nodes_per_row_IDX + 1) * (nodes_per_col_IDX + 1) - 1; // How many Nodes there are. Always decremented -1, as this is used as an index.
	int mouseX = 0; // Where the mouse X position gets stored. Updates everytime the mouse is moved. In the range of your display width.
	int mouseY = 0; // Where the mouse Y position gets stored. Updates everytime the mouse is moved. In the range of your display height.
	int xPos = 0;  // Where the window X position gets stored. Updates everytime the mouse is moved. In the range of your display width.
	int yPos = 0;  // Where the window X position gets stored. Updates everytime the mouse is moved. In the range of your display width.

	// Decrements the window position from the display mouse position in order to get the below values.
	int newX = 0; // Where the mouse X position relative to the window's position. Updates everytime the mouse is moved. In the range of the window width.
	int newY = 0; // Where the mouse Y position relative to the window's position. Updates everytime the mouse is moved. In the range of the window height.

	// Misc.
	const char* TITLE = "A* Path Finding Algorithm"; // Window title.

public:

	// Environment
	Engine(); // Engine constructor with default values and no arguments.
	Engine(int width, int height); // Engine constructor with default title but passing the window's width and height as arguments.
	Engine(const char* title, int width, int height); // Engine constructor without default values, window's title, width, and height as arguments.
	~Engine();
	int load(); // Sets up the environment.
	void start(); // Runs the algorithm.
	int close(); // Closes the environment.
	void reset(); // Resets the grid.
	void changeAlgorithm(); // Changes the algorithm.
	void toggleCostMode();
	void toggleViewOnly();
	void toggleTargetMode();
	void toggleStartMode();
	void handleMouse(bool button_down, int button_pressed);
	void handleMouseClick();
	void updateMousePosition();

	// Validation
	bool isNodeValid(int width, int height);
	bool isValidMousePosition();
	bool isValidLeftNeighbor(int index);
	bool isValidTopNeighbor(int index);
	bool isValidRightNeighbor(int index);
	bool isValidBottomNeighbor(int index);

	// SDL
	void updateGrid(); // Updates the grid.
	void updateRenderer(); // Updates the renderer.
	void clearWindow(); // Clears the window.
	void clearVisited(); // Clears visited nodes.

	// Getters
	SDL_Event getEvent();
	int getWidth();
	int getHeight();
};