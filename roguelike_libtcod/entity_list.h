#ifndef ENTITY_LIST_H_12_9_21
#define ENTITY_LIST_H_12_9_21

#include <iostream>
#include <libtcod.h>
#include "game_map.h"

class Entity;

/*
* A container class to hold and manage the list of entities.
* Must be initialized with references to:
*	-map: allows EntityList to check whether an entity is in fov for rendering
*	-player: allows EntityList to distinguish which entity is the player when 
*		calling updates on other entities
*/
class EntityList {
public:
	EntityList(GameMap* m, Entity* p);
	~EntityList();

	std::vector<Entity*> entities;

	// Push back a new entity to internal entities list
	void add_entity(Entity* e);
	// Remove an entity from the list. Should throw an error if the entity is not found
	void remove_entity(Entity* e);
	// Remove all entities from the list
	void clear_entities();

	// Return a reference to the first entity at(x, y). Returns nullptr if none exists. 
	Entity* get_entity_at(int x, int y);
	// Return a reference to the entity at (x, y) for which the property blocks = true
	Entity* check_collision_at(int x, int y);

	// Update all non-player entities
	void update();
	// Render all entities within map fov to the console
	void render(tcod::Console* console);
private:
	GameMap* map;
	Entity* player;
};

#endif

