#ifndef ENGINE_H_11_19_21
#define ENGINE_H_11_19_21

#include <iostream>
#include <libtcod.h>
#include <SDL.h>
#include "command.h"
#include "constants.h"
#include "entity.h"
#include "game_map.h"
#include "input_handler.h"
#include "tile.h"



class Engine {
public:
	bool quit = false;
	bool recompute_fov = true;

	Engine(int argc, char* argv[]);
	~Engine();

	// Execute all pending commands and state changes
	void update();
	// Execute the player command retrieved from the input handler
	void execute_player_command(Command* command);

	void render();
	void render_entities();
	void delete_entities(); // Entities are dynamically allocated, so must be del
private:
	tcod::Console console;
	tcod::ContextPtr context;

	InputHandler input;

	GameMap map = GameMap(DEF_WIDTH, DEF_HEIGHT, DEF_WALL_GRAPHIC, DEF_FLOOR_GRAPHIC, DEF_BSP_PARAMS);

	std::vector<Entity*> entities;
	Entity* player = new Entity(2, 2, '@', tcod::ColorRGB(TCOD_white), "The Great Borealis");

	// Initialize the tcod::Context and return a pointer to it to be saved 
	tcod::ContextPtr initialize_context(tcod::Console* console, int argc, char* argv[]);
};


#endif