#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

struct colors {
	constexpr static Uint8 passable[4] = { 0x00, 0xFF, 0x00, 0xFF };
	constexpr static Uint8 impassable[4] = { 0xFF, 0x00, 0x00, 0xFF };
	constexpr static Uint8 start[4] = { 0xFF, 0xFF, 0x00, 0xFF };
	constexpr static Uint8 target[4] = { 0x00, 0xFF, 0xFF, 0xFF };
	constexpr static Uint8 visited[4] = { 125, 5, 88, 255 };
};

#pragma once
class Node
{
private:
	bool passable;
	bool target = false;
	bool start = false;
	int x;
	int y;
	int width = 40;
	int height = 40;

	Uint8 red;
	Uint8 green;
	Uint8 blue;
	Uint8 alpha;
	
	const char* type;

public:
	bool visited = false;
	int cameFrom = 0;

	int index = 1245;

	Node(const char* type, int x, int y);
	~Node();
	int getX();
	int getY();
	int getWidth();
	int getHeight();


	const char* getType();
	Uint8 getRed() { return this->red; };
	Uint8 getGreen() { return this->green; };
	Uint8 getBlue() { return this->blue; };
	Uint8 getAlpha() { return this->alpha; };

	void setType(const char* type);
	void setColors(const char* type);
	void setRed(Uint8 red);
	void setGreen(Uint8 green);
	void setBlue(Uint8 blue);
	void setAlpha(Uint8 alpha);
};

