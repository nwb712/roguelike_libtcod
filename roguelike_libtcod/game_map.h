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
	// The default constructor pulls data from constants.h to initialize vars
	GameMap();
	GameMap(int w, int h, TileGraphic wl, TileGraphic flr, BSPParams bsp_params);
	~GameMap();

	void initialize_tiles(int width, int height, TileGraphic t);
	Tile* get_tile(int x, int y);
	void render_tiles(tcod::Console* console);

	// Returns true if the given coordinate is within the bounds of the tile map
	bool in_bounds(int x, int y);

	bool is_passable(int x, int y);
	void set_passable(int x, int y, bool pass) { get_tile(x, y)->set_passable(pass); }
	void toggle_passable(int x, int y);

	bool is_transparent(int x, int y);
	void set_transparent(int x, int y, bool tran) { get_tile(x, y)->set_transparent(tran); }
	void toggle_transparent(int x, int y);

	bool is_dark(int x, int y);
	void set_dark(int x, int y, bool dark) { get_tile(x, y)->set_dark(dark); }
	void toggle_dark(int x, int y);

	/* 
	* Generate the bsp tree using the given parameters. Called during construction
	* using BSPParams. Specifics of BSP can be found in libtcod documentation.
	*/
	void bsp_generate(int depth, int minh, int minv, float max_h_ratio, float max_v_ratio);
	
	/*
	 * Retrieve a reference to the tile at the specified location, then set the
	 * following: transparent = true; passable = true; graphic = wall_graphic
	 */
	void dig(int x, int y);

	// Execute the dig function at all points within the rect desc by (x, y, w, h)
	void dig_room(int x, int y, int w, int h);
	
	/*
	 * Execute dig() function to dig a tunnell from one point to another; either
	 * horizontally or vertically first.
	 * dir = true (default) -> horizontal, then vertical
	 * dir = false -> vertical, then horizontal
	 *
	 * Note that reversing the two points also has the same effect as switching dir.
	 */
	void dig_tunnel(int x1, int y1, int x2, int y2, bool dir = true);
private:
	int width = 1;
	int height = 1;
	TileGraphic wall_graphic;
	TileGraphic floor_graphic;
	Tile *tiles;
	BSPParams bsp_params;
	TCODBsp *bsp;
	TCODRandom* randomizer;
};

#endif
