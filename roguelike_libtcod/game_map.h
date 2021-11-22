#ifndef GAME_MAP_H_11_15_21
#define GAME_MAP_H_11_15_21

#include <iostream>
#include <libtcod.h>
#include "tile.h"



// Holds all of the parameters needed to initialize the GameMap
struct BSPParams {
	int depth = 3;
	int minh = 4;
	int minv = 4;
	float max_h_ratio = 0.75;
	float max_v_ratio = 0.75;
};

// A rectangle struct to hold room information
struct Rect {
	Rect() {}
	Rect(int xx, int yy, int ww, int hh): x(xx), y(yy), w(ww), h(hh) {}
	int center_x() { return x + (int)w / 2; }
	int center_y() { return y + (int)h / 2; }
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

	/* 
	* intitialize the fov_map to the proper dimensions then read in walkable and
	* transparency data so that fov can be properly computed. FOV functions now 
	* are essentially just a wrapper around the TCODMap functionality. Considering 
	* separating fov into its own class at some point, but it works just fine 
	* within GameMap for right now. 
	*/
	void initialize_fov_map();
	// Compute the fov at a given x, y location (usually the player position)
	void compute_fov(int x, int y, int radius = 6, bool light_walls = true, TCOD_fov_algorithm_t algo = FOV_BASIC);
	// Return whether a cell at a given x, y location is within fov
	bool is_in_fov(int x, int y) { return fov_map->isInFov(x, y); }

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

	// Custom callback for TCODBsp
	class BSPCallBack : public ITCODBspCallback {
	public:
		BSPCallBack(GameMap* m) : map(m) {}
		bool visitNode(TCODBsp* node, void* userData);
	private:
		GameMap* map;
	};

	/* 
	* Generate the bsp tree using the given parameters. Called during construction
	* using BSPParams. Specifics of BSP can be found in libtcod documentation.
	*/
	void bsp_generate(int depth, int minh, int minv, float max_h_ratio, float max_v_ratio);
	
	/*
	* Generate rooms within each bsp-generated rectangle
	*/
	void bsp_generate_rooms(int minw, int minh, int maxw, int maxh);

	Rect get_room(int index);

	/*
	 * Retrieve a reference to the tile at the specified location, then set the
	 * following: transparent = true; passable = true; graphic = wall_graphic
	 */
	void dig(int x, int y);

	// Execute the dig function at all points within the rect desc by (x, y, w, h)
	void dig_room(int x, int y, int w, int h);
	void dig_room(Rect rm);
	
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

	TCODMap* fov_map;

	BSPParams bsp_params;
	TCODBsp *bsp;

	TCODRandom* randomizer;

	std::vector<Rect> rects;
	std::vector<Rect> rooms;
};

#endif
