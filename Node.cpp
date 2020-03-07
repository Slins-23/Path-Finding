#include "Node.h"

Node::Node(const char* type, int x, int y) {
	this->x = x;
	this->y = y;

	setType(type);
};

Node::~Node() {
}

int Node::getX() {
	return this->x;
};

int Node::getY() {
	return this->y;
};

int Node::getWidth() {
	return this->width;
};

int Node::getHeight() {
	return this->height;
}

const char* Node::getType() {
	return this->type;
}

void Node::setType(const char* type) {
	this->type = type;

	if (type == "passable") {
		this->passable = true;
		this->start = false;
		this->target = false;
		setRed(colors::passable[0]);
		setGreen(colors::passable[1]);
		setBlue(colors::passable[2]);
		setAlpha(colors::passable[3]);
	}
	else if (type == "impassable") {
		this->passable = false;
		this->start = false;
		this->target = false;
		setRed(colors::impassable[0]);
		setGreen(colors::impassable[1]);
		setBlue(colors::impassable[2]);
		setAlpha(colors::impassable[3]);
	}
	else if (type == "start") {
		this->passable = true;
		this->start = true;
		this->target = false;
		setRed(colors::start[0]);
		setGreen(colors::start[1]);
		setBlue(colors::start[2]);
		setAlpha(colors::start[3]);
	}
	else if (type == "target") {
		this->passable = true;
		this->start = false;
		this->target = true;
		setRed(colors::target[0]);
		setGreen(colors::target[1]);
		setBlue(colors::target[2]);
		setAlpha(colors::target[3]);
	}
	else if (type == "visited") {
		this->passable = true;
		this->start = false;
		this->target = false;
		this->visited = true;
		setRed(colors::visited[0]);
		setGreen(colors::visited[1]);
		setBlue(colors::visited[2]);
		setAlpha(colors::visited[3]);
	}
	else {
		std::cout << "Set type error" << std::endl;
		this->passable = false;
		this->start = false;
		this->target = false;
		this->red = 0x00;
		this->green = 0x00;
		this->red = 0x00;
		this->alpha = 0x00;
	}
}

void Node::setRed(Uint8 r) {
	this->red = r;
}

void Node::setGreen(Uint8 g) {
	this->green = g;
}

void Node::setBlue(Uint8 b) {
	this->blue = b;
}

void Node::setAlpha(Uint8 a) {
	this->alpha = a;
}

void Node::setColors(const char* type) {
	if (type == "passable") {
		setRed(colors::passable[0]);
		setGreen(colors::passable[1]);
		setBlue(colors::passable[2]);
		setAlpha(colors::passable[3]);
	}
	else if (type == "impassable") {
		setRed(colors::impassable[0]);
		setGreen(colors::impassable[1]);
		setBlue(colors::impassable[2]);
		setAlpha(colors::impassable[3]);
	}
	else if (type == "start") {
		setRed(colors::start[0]);
		setGreen(colors::start[1]);
		setBlue(colors::start[2]);
		setAlpha(colors::start[3]);
	}
	else if (type == "target") {
		setRed(colors::target[0]);
		setGreen(colors::target[1]);
		setBlue(colors::target[2]);
		setAlpha(colors::target[3]);
	}
	else if (type == "visited") {
		setRed(colors::visited[0]);
		setGreen(colors::visited[1]);
		setBlue(colors::visited[2]);
		setAlpha(colors::visited[3]);
	}
	else {
		std::cout << "Set colors error" << std::endl;
		setRed(0x00);
		setGreen(0x00);
		setBlue(0x00);
		setAlpha(0x00);
	}
}