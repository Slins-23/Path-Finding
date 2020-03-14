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

// Node struct. All Nodes are instances of this struct.
#pragma once
struct Node
{
	Node(const char* type, int x, int y); // The Node constructor takes in the type of the Node and, respectively, the X and Y coordinates, relative to the display.
	~Node();

	// Integers
	int x; // Node X position within the window.
	int y; // Node Y position within the window.
	int width = 40; // Node's width.
	int height = 40; // Node's height.
	int index = 12345; // Node's index.

	// Uint8
	Uint8 red; // Node's red color.
	Uint8 green; // Node's green color.
	Uint8 blue; // Node's blue color.
	Uint8 alpha; // Node's alpha value.
	
	// Misc.
	const char* type; // Node's type. ("passable" || "impassable" || "start" || "target" || "visited")

	// Nodes
	Node* came_from = nullptr; // Node from which this Node came from.

	// Floats
	int cost = 1; // Node's cost value / How long it takes to pass through this Node
	float h = INFINITY;
	float g = INFINITY;
	float f = INFINITY;

	const char* getType(); // Returns the Node's type. ("passable" || "impassable" || "start" || "target" || "visited")

	// Setters
	void setType(const char* type); // Sets the Node's type. Argument should be ("passable" || "impassable" || "start" || "target" || "visited").
	void setColors(const char* type); // Sets the Node's color to the specified type, but don't change the Node's type itself. Argument should be ("passable" || "impassable" || "start" || "target" || "visited").
};