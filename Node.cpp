#include "Node.h"

Node::Node(const char* type, int x, int y) {
	this->x = x;
	this->y = y;

	setType(type);
};

Node::~Node() {
}

const char* Node::getType() {
	return this->type;
}

void Node::setType(const char* type) {
	this->type = type;

	if (type == "passable") {
		this->cost = 1;
		this->g = INFINITY;
		this->f = INFINITY;
		this->red = colors::passable[0];
		this->green = colors::passable[1];
		this->blue = colors::passable[2];
		this->alpha = colors::passable[3];
	}
	else if (type == "impassable") {
		this->cost = 5;
		this->red = colors::impassable[0];
		this->green = colors::impassable[1];
		this->blue = colors::impassable[2];
		this->alpha = colors::impassable[3];
	}
	else if (type == "start") {
		this->red = colors::start[0];
		this->green = colors::start[1];
		this->blue = colors::start[2];
		this->alpha = colors::start[3];
	}
	else if (type == "target") {
		this->red = colors::target[0];
		this->green = colors::target[1];
		this->blue = colors::target[2];
		this->alpha = colors::target[3];
	}
	else if (type == "visited") {
		this->red = colors::visited[0];
		this->green = colors::visited[1];
		this->blue = colors::visited[2];
		this->alpha = colors::visited[3];
	}
	else {
		std::cout << "Set type error" << std::endl;
		throw;
		this->red = 0x00;
		this->green = 0x00;
		this->red = 0x00;
		this->alpha = 0x00;
	}
}

void Node::setColors(const char* type) {

	if (type == "passable") {
		this->red = colors::passable[0];
		this->green = colors::passable[1];
		this->blue = colors::passable[2];
		this->alpha = colors::passable[3];
	}
	else if (type == "impassable") {
		this->red = colors::impassable[0];
		this->green = colors::impassable[1];
		this->blue = colors::impassable[2];
		this->alpha = colors::impassable[3];
	}
	else if (type == "start") {
		this->red = colors::start[0];
		this->green = colors::start[1];
		this->blue = colors::start[2];
		this->alpha = colors::start[3];
	}
	else if (type == "target") {
		this->red = colors::target[0];
		this->green = colors::target[1];
		this->blue = colors::target[2];
		this->alpha = colors::target[3];
	}
	else if (type == "visited") {
		this->red = colors::visited[0];
		this->green = colors::visited[1];
		this->blue = colors::visited[2];
		this->alpha = colors::visited[3];
	}
	else {
		//std::cout << "Set type error" << std::endl;
		throw "Color type error";
		this->red = 0x00;
		this->green = 0x00;
		this->red = 0x00;
		this->alpha = 0x00;
	}
}