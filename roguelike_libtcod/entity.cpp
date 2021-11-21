#include <iostream>
#include <libtcod.h>
#include "entity.h"

Entity::Entity() {
	xpos = 0;
	ypos = 0;
	chr = ' ';
}

Entity::Entity(int x, int y, char c, tcod::ColorRGB clr, std::string n) {
	xpos = x;
	ypos = y;
	chr = c;
	color = clr;
	name = n;
}

// Move the entity by a given amount
void Entity::move(int dx, int dy) {
	xpos += dx;
	ypos += dy;
}

void Entity::wait() {
	std::cout << name << " waits." << std::endl;
	return;
}

std::string Entity::getName() { return name; }
void Entity::setName(std::string n) { name = n; }

int Entity::getX() { return xpos; }
int Entity::getY() { return ypos; }

char Entity::getChar() {
	return chr;
}
void Entity::setChar(char c) {
	chr = c;
}

tcod::ColorRGB Entity::getColor() {
	return color;
}
void Entity::setColor(tcod::ColorRGB color) {
	color = color;
}