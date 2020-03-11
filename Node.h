#include <iostream>
#include <sstream>
#include <SDL.h>
#include "SDL_ttf.h"
#include <limits>
#include <set>

// Colors for the Nodes, based on types.
struct colors {
	constexpr static Uint8 passable[4] = { 0x00, 0xFF, 0x00, 0xFF }; // Passable - Green
	constexpr static Uint8 impassable[4] = { 0xFF, 0x00, 0x00, 0xFF }; // Impassable - Red
	constexpr static Uint8 start[4] = { 0xFF, 0xFF, 0x00, 0xFF }; // Start - Yellow
	constexpr static Uint8 target[4] = { 0x00, 0xFF, 0xFF, 0xFF }; // Target - Blue
	constexpr static Uint8 visited[4] = { 125, 5, 88, 255 }; // Visited - Purple
};

// Node class. All Nodes are instances of this class.
#pragma once
class Node
{
private:
	bool passable; // Whether the Node can be passed through.
	bool target = false; // If this Node is the target point.
	bool start = false; // If this Node is the start point.
	int x; // Node X position within the window.
	int y; // Node Y position within the window.
	int width = 40; // Node's width.
	int height = 40; // Node's height.

	Uint8 red; // Node's red color.
	Uint8 green; // Node's green color.
	Uint8 blue; // Node's blue color.
	Uint8 alpha; // Node's alpha value.
	
	const char* type; // Node's type. ("passable" || "impassable" || "start" || "target" || "visited")

public:
	// Booleans
	bool visited = false; // Whether this Node was visited by the algorithm.

	// Integers
	int cameFrom = 0; // Index of the Node from which this Node came from.
	int index = 1245; // Node's index.

	// Floats
	float cost = 1; // Node's cost value / How long it takes to pass through this Node
	float cost_so_far = INFINITY; // Cost from the starting point to this Node.
	float fLocalGoal = INFINITY; // Should be cost
	float fGlobalGoal = INFINITY; // Should be cost_so_far

	// Node
	Node(const char* type, int x, int y); // The Node constructor takes in the type of the Node and, respectively, the X and Y coordinates, relative to the display.
	~Node();

	// Getters
	int getX(); // Returns the Node's X position.
	int getY(); // Returns the Node's Y position.
	int getWidth(); // Returns the Node's width.
	int getHeight(); // Returns the Node's height.
	Uint8 getRed() { return this->red; }; // Returns the Node's red color.
	Uint8 getGreen() { return this->green; }; // Returns the Node's green color.
	Uint8 getBlue() { return this->blue; }; // Returns the Node's blue color.
	Uint8 getAlpha() { return this->alpha; }; // Returns the Node's alpha value.
	const char* getType(); // Returns the Node's type. ("passable" || "impassable" || "start" || "target" || "visited")

	// Setters
	void setRed(Uint8 red); // Sets the Node's red color. Argument should be in the ranges (0-255 || 0x00-0xFF).
	void setGreen(Uint8 green); // Sets the Node's green color. Argument should be in the range (0-255 || 0x00-0xFF).
	void setBlue(Uint8 blue); // Sets the Node's blue color. Argument should be in the range (0-255 || 0x00-0xFF).
	void setAlpha(Uint8 alpha); // Sets the Node's alpha value. Argument should be in the range (0-255 || 0x00-0xFF).
	void setType(const char* type); // Sets the Node's type. Argument should be ("passable" || "impassable" || "start" || "target" || "visited").
	void setColors(const char* type); // Sets the Node's color to the specified type, but don't change the Node's type itself. Argument should be ("passable" || "impassable" || "start" || "target" || "visited").

	bool operator <(const Node& other) const {
		return this->cost_so_far < other.cost_so_far;
	}

	bool operator >(const Node& other) const {
		return this->cost_so_far > other.cost_so_far;
	}

	bool operator ==(const Node& other) const {
		return this->cost_so_far == other.cost_so_far;
	}
};

