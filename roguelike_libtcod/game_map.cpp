#include <iostream>
#include <libtcod.h>
#include "game_map.h"
#include "tile.h"



GameMap::GameMap(int w, int h, TileGraphic wl, TileGraphic flr, BSPParams bsp_params) {
	width = w;
	height = h;

	wall_graphic = wl;
	floor_graphic = flr;

	int arr_size = (int)(width * height);
	tiles = new Tile[arr_size];
	bsp = new TCODBsp(0, 0, w, h);
	randomizer = new TCODRandom;

	bsp_generate(bsp_params.depth, bsp_params.minh, 
		bsp_params.minv, bsp_params.max_h_ratio, bsp_params.max_v_ratio);
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
	int pos = int(x + y * width);
	return (pos < sizeof(tiles));
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

void GameMap::dig(int x, int y) {
	Tile* t = GameMap::get_tile(x, y);
	if (!t->is_transparent()) {
		t->toggle_transparent();
	}
	if (!t->is_passable()) {
		t->toggle_passable();
	}
	t->set_graphic(&wall_graphic);
}

void GameMap::dig_room(int x, int y, int w, int h) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			dig(x + i, y + j);
		}
	}
}

void GameMap::dig_tunnel(int x1, int y1, int x2, int y2, bool dir) {
	if (dir) {
		int x_dif = x2 - x1;
		for (int i = 0; i != x_dif; i += (x_dif > 0) - (x_dif < 0)) {
			dig(x1 + i, y1);
		}
		int y_dif = x2 - x1;
		for (int j = 0; j != y_dif; j += (y_dif > 0) - (y_dif < 0)) {
			dig(x1 + x_dif, y1 + j);
		}
	}
	else {
		int y_dif = y2 - y1;
		for (int j = 0; j != y_dif; j += (y_dif > 0) - (y_dif < 0)) {
			dig(x1, y1 + j);
		}
		int x_dif = x2 - x1;
		for (int i = 0; i != x_dif; i += (x_dif > 0) - (x_dif < 0)) {
			dig(x1 + i, y1 + y_dif);
		}
	}
}