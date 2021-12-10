#ifndef ENGINE_H_11_19_21
#define ENGINE_H_11_19_21

#include <iostream>
#include <libtcod.h>
#include <SDL.h>
#include "entity.h"
#include "entity_list.h"
#include "game_map.h"
#include "command.h"
#include "constants.h"



enum GameState {
	STARTUP,
	IDLE,
	NEW_TURN,
	VICTORY,
	DEFEAT
};



class Engine {
public:
	bool quit = false;
	bool recompute_fov = true;

	Engine(int argc, char* argv[]);
	~Engine();


	// Place an enemy at the specified location
	void add_enemy(int x, int y);
	// Place enemies in all rooms on the game map
	void populate_enemies(int max_pop);

	// Execute all pending commands and state changes
	void update();
	// Render all elements to the console
	void render();
private:
	GameState game_state = STARTUP;

	tcod::Console console;
	tcod::ContextPtr context;

	GameMap* map;

	EntityList* entities;
	Entity* player;

	// Initialize the tcod::Context and return a pointer to it to be saved 
	tcod::ContextPtr initialize_context(tcod::Console* console, int argc, char* argv[]);
};


#endif