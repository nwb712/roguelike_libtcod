#include <iostream>
#include <libtcod.h>
#include "game_map.h"
#include "tile.h"

/*
* TODO Need to create a list to store the rectangles representing the rooms. Not sure
*	yet whether I will separate a room into its own type using a struct.
* TODO: Address the possible overflow in width * height calculations. It seems 
*	safe to leave in place for now as I don't believe I will ever exceed 
	2147483647 Tiles on one GameMap
*/

GameMap::GameMap(int w, int h, TileGraphic wl, TileGraphic flr) {
	width = w;
	height = h;
	wall_graphic = wl;
	floor_graphic = flr;
	tiles = new Tile[width * height];
	std::cout << sizeof(tiles);
	bsp = new TCODBsp(0, 0, w, h);
	randomizer = new TCODRandom;
}
GameMap::~GameMap() {
	delete tiles; delete bsp; delete randomizer;
}


Tile* GameMap::get_tile(int x, int y) {
	return &tiles[x + width * y];
}

void GameMap::render_tiles(tcod::Console* c) {
	Tile t;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			t = tiles[i + j * width];
			TCOD_Console  con = (TCOD_Console)*c; // Retrieve a reference to TCOD_Console pointer
			if (!t.get_data().dark) {
				TCOD_console_set_char_background(&con, i, j, t.get_graphic().color_bg, TCOD_BKGND_SET);
			}
			else {
				TCOD_console_set_char_background(&con, i, j, t.get_graphic().color_dark_bg, TCOD_BKGND_SET);
			}
		}
	}
}

bool GameMap::in_bounds(int x, int y) {
	return (x + y * width < sizeof(tiles));
}

bool GameMap::is_passable(int x, int y) {
	return tiles[x + y * width].is_passable();
}
void GameMap::toggle_passable(int x, int y) {
	tiles[x + y * width].toggle_passable();
}


bool GameMap::is_transparent(int x, int y) {
	return tiles[x + y * width].is_transparent();
}
void GameMap::toggle_transparent(int x, int y) {
	Tile* t = get_tile(x, y);
}


bool GameMap::is_dark(int x, int y) {
	return tiles[x + y * width].is_dark();
}
void GameMap::toggle_dark(int x, int y) {
	tiles[x + y * width].toggle_dark();
}

void GameMap::bsp_generate(int depth, int minh, int minv, float max_h_ratio, float max_v_ratio) {
	
}

void GameMap::generate_room(int x, int y, int w, int h) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			Tile* t = GameMap::get_tile(x + i, y + j);
			if (!t->is_transparent()) {
				t->toggle_transparent();
			}
			if (!t->is_passable()) {
				t->toggle_passable();
			}
			t->set_graphic(&wall_graphic);
		}
	}
}