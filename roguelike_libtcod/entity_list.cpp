#include <iostream>
#include <libtcod.h>
#include "entity.h"
#include "entity_list.h"
#include "game_map.h"



EntityList::EntityList(GameMap* m, Entity* p) {
	map = m;
	player = p;
}



EntityList::~EntityList() {
	clear_entities();
}



void EntityList::add_entity(Entity* e) {
	entities.push_back(e);
}



void EntityList::remove_entity(Entity* e) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i] == e) {
			auto it = entities.begin();
			for (int j = 0; j < i; j++) {
				it++;
			}
			entities.erase(it);
		}
	}
}



void EntityList::clear_entities() {
	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
}



Entity* EntityList::get_entity_at(int x, int y) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getX() == x && entities[i]->getY() == y) {
			return entities[i];
		}
	}
	return nullptr;
}



Entity* EntityList::check_collision_at(int x, int y) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getX() == x && entities[i]->getY() == y) {
			if (entities[i]->get_blocks()) {
				return entities[i];
			}
		}
	}
	return nullptr;
}



void EntityList::update() {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i] != player) {
			entities[i]->update(map, entities);
		}
	}
}



void EntityList::render(tcod::Console* console) {
	Entity* e;
	for (int i = 0; i < entities.size(); i++) {
		e = entities[i];
		if (map->is_in_fov(e->getX(), e->getY())) {
			tcod::print(*console, { e->getX(), e->getY() }, std::string(1, e->getChar()), e->getColor(), std::nullopt);
		}
	}
}