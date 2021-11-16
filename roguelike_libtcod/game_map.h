#ifndef GAME_MAP_H_11_15_21
#define GAME_MAP_H_11_15_21

#include <iostream>
#include <libtcod.h>
#include "tile.h"

class GameMap {
public:
	GameMap(int w, int h, TileGraphic wl, TileGraphic flr);
	~GameMap();
	Tile* get_tile(int x, int y);
	void render_tiles(tcod::Console* console);

	bool in_bounds(int x, int y);

	bool is_passable(int x, int y);
	void toggle_passable(int x, int y);

	bool is_transparent(int x, int y);
	void toggle_transparent(int x, int y);

	bool is_dark(int x, int y);
	void toggle_dark(int x, int y);

	void bsp_generate(int depth, int minh, int minv, float max_h_ratio, float max_v_ratio);

	void generate_room(int x, int y, int w, int h);
	//void generate_tunnel(int x1, int y1, int x2, int y2);
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
