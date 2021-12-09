#include <iostream>
#include <libtcod.h>
#include "entity.h"

Entity::Entity() {
	blocks = true;
	xpos = 0;
	ypos = 0;
	chr = ' ';
}

Entity::Entity(int x, int y, char c, tcod::ColorRGB clr, std::string n, bool blcks){
	blocks = blcks;
	xpos = x;
	ypos = y;
	chr = c;
	color = clr;
	name = n;
}



Entity* Entity::check_collision(GameMap* map) {

}



void Entity::execute_command(Command* command, GameMap* map) {
	if (command) {
		if (command->get_type() == CommandType::quit_command) {
			quit = true;
		}
		if (command->get_type() == CommandType::move_command) {
			Entity* e = check_collision(map);
			recompute_fov = true; // Need to recompute map fov after player move
			if (e) {
				std::cout << "You verbally assault the " << e->getName() << std::endl;
			}
			else if (map->is_passable(getX() + command->getDx(), getY() + command->getDy())) {
				Entity self = *this;
				command->execute(self);
			}
			else {
				// May be a good idea to create a message class
				std::cout << "You walk into a wall.\n";
			}
		}
	}
}



void Entity::update(GameMap* map, std::vector<Entity*>& entities) {
	if (ai) {
		Command* command;
		ai->update(map);
		command = ai->get_command();
	}
	else {
		std::cout << name << " update called with no ai componet" << std::endl;
	}
	
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