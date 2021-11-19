#ifndef GAME_MAP_H_11_15_21
#define GAME_MAP_H_11_15_21

#include <iostream>
#include <libtcod.h>
#include "tile.h"

/*
* TODO Need to create a list to store the rectangles representing the rooms.
* TODO Finish implementing BSP generation. Room and tunnel digging are functional,
*	so the only thing left to do is get the tcod bsp functionality working and 
*	somehow export that information to a list of rooms. For now, rooms will simply
*	hollow out the whole bsp rect, but would like to allow for a size range and 
*	random placement within the bsp node. I got it working in GDScript, so 
*	reference the Godot project if there are difficulties.
*/

// Holds all of the parameters needed to initialize the GameMap
struct BSPParams {
	int depth = 3;
	int minh = 4;
	int minv = 4;
	float max_h_ratio = 0.75;
	float max_v_ratio = 0.75;
};

// A rectangle struct to hold room information
struct Room {
	Room(int xx, int yy, int ww, int hh): x(xx), y(yy), w(ww), h(hh) {}
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};


/*
* Holds all the information necessary for generation and management of the game
* map. Tiles can be modified using methods associated with this class.
*/
class GameMap {
public:
	GameMap(int w, int h, TileGraphic wl, TileGraphic flr, BSPParams bsp_params);
	~GameMap();
	Tile* get_tile(int x, int y);
	void render_tiles(tcod::Console* console);

	// Returns true if the given coordinate is within the bounds of the tile map
	bool in_bounds(int x, int y);

	bool is_passable(int x, int y);
	void toggle_passable(int x, int y);

	bool is_transparent(int x, int y);
	void toggle_transparent(int x, int y);

	bool is_dark(int x, int y);
	void toggle_dark(int x, int y);

	/* 
	* Generate the bsp tree using the given parameters. Called during construction
	* using BSPParams. Specifics of BSP can be found in libtcod documentation.
	*/
	void bsp_generate(int depth, int minh, int minv, float max_h_ratio, float max_v_ratio);
	
	/*
	* Set the state of a tile in a specific location on the map to both 
	* transparent and passable. Also update the graphic to the floor graphic.
	*/
	void dig(int x, int y);
	// Dig out a rectangular area of tiles
	void dig_room(int x, int y, int w, int h);
	// Dig a tunnel from one point to another; dir = true -> horizontal first
	void dig_tunnel(int x1, int y1, int x2, int y2, bool dir = true);
private:
	int width = 1;
	int height = 1;
	TileGraphic wall_graphic;
	TileGraphic floor_graphic;
	Tile *tiles;
	TCODBsp *bsp;
	TCODRandom* randomizer;
};

#endif
