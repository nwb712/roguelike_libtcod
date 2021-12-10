#include <iostream>
#include <libtcod.h>
#include "entity.h"



Entity::~Entity() {
	if (ai) {
		delete ai;
	}
	if (destructable) {
		delete destructable;
	}
	if (combatant) {
		delete combatant;
	}
}



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



Entity* Entity::check_collision(int x, int y, GameMap* map, std::vector<Entity*> entities) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getX() == x && entities[i]->getY() == y) {
			return entities[i];
		}
	}
	return nullptr;
}



void Entity::execute_command(Command* command, GameMap* map, std::vector<Entity*> entities) {
	if (command) {
		if (command->get_type() == CommandType::quit_command) {
			;
		}
		if (command->get_type() == CommandType::move_command) {
			Entity* e = check_collision(getX() + command->getDx(), getY() + command->getDy(), map, entities);
			if (e) {
				std::cout << name << " verbally assaults the " << e->getName() << std::endl;
			}
			else if (map->is_passable(getX() + command->getDx(), getY() + command->getDy())) {
				command->execute(*this);
				map->recompute_fov = true;
			}
			else {
				std::cout << name <<" walks into a wall.\n";
			}
		}
	}
}



void Entity::update(GameMap* map, std::vector<Entity*> entities) {
	Command* command = nullptr;
	if (ai) {
		command = ai->update(map, entities);
		execute_command(command, map, entities);
	}
}



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