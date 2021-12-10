#include <iostream>
#include <libtcod.h>
#include "constants.h"
#include "entity.h"
#include "game_map.h"
#include "tile.h"



GameMap::GameMap() : width(DEF_WIDTH), height(DEF_HEIGHT) {
	wall_graphic = TileGraphic(DEF_WALL_GRAPHIC);
	floor_graphic = TileGraphic(DEF_FLOOR_GRAPHIC);
	bsp_params = DEF_BSP_PARAMS;

	initialize_tiles(width, height, wall_graphic);

	bsp = new TCODBsp(0, 0, DEF_WIDTH, DEF_HEIGHT);
	randomizer = new TCODRandom;
	bsp_generate(bsp_params.depth, bsp_params.minh,
		bsp_params.minv, bsp_params.max_h_ratio, bsp_params.max_v_ratio);

	initialize_fov_map();
}



GameMap::GameMap(int w, int h, TileGraphic wl, TileGraphic flr, BSPParams params) {
	width = w;
	height = h;

	wall_graphic = wl;
	floor_graphic = flr;

	initialize_tiles(w, h, wall_graphic);

	bsp = new TCODBsp(0, 0, w, h);
	randomizer = new TCODRandom;
	bsp_params = params;
	bsp_generate(bsp_params.depth, bsp_params.minh, 
		bsp_params.minv, bsp_params.max_h_ratio, bsp_params.max_v_ratio);


	initialize_fov_map();
}
GameMap::~GameMap() {
	delete tiles; delete bsp; delete randomizer; delete fov_map;
}



void GameMap::initialize_tiles(int width, int height, TileGraphic t) {
	long arr_size = width * height;
	tiles = new Tile[(int)arr_size];
	
	for (int i = 0; i < arr_size; i++) {
		tiles[i].set_graphic(t);
	}
}



Tile* GameMap::get_tile(int x, int y) {
	return &tiles[x + width * y];
}



// Render all tiles to the console
void GameMap::render_tiles(tcod::Console* c) {
	Tile* t;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			t = &tiles[i + j * width];
			TCOD_Console  con = (TCOD_Console)*c; // Retrieve a reference to TCOD_Console pointer
			if (t->get_data().explored) {
				if (!t->get_data().dark) {
					TCOD_console_put_char_ex(&con, i, j, t->get_graphic().c,
						t->get_graphic().color_fg, t->get_graphic().color_bg);
				}
				else {
					TCOD_console_put_char_ex(&con, i, j, t->get_graphic().c,
						t->get_graphic().color_dark_fg, t->get_graphic().color_dark_bg);
				}
			}
		}
	}
}



void GameMap::initialize_fov_map() {
	fov_map = new TCODMap(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			fov_map->setProperties(i, j, get_tile(i, j)->get_data().transparent, 
				get_tile(i, j)->get_data().passable);
		}
	}
}



void GameMap::compute_fov(int x, int y, int radius, bool light_walls, 
		TCOD_fov_algorithm_t algo) {
	fov_map->computeFov(x, y, radius, light_walls, algo);
	Tile* t;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			t = get_tile(i, j);
			bool in_fov = is_in_fov(i, j);
			if (in_fov) {
				t->set_explored(true);
				t->set_dark(false);
			}
			if (!in_fov && t->get_data().explored) {
				t->set_dark(true);
			}
		}
	}
	recompute_fov = false;
}



// Return whether (x, y) is in console bounds
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



bool GameMap::BSPCallBack::visitNode(TCODBsp* node, void* userData) {
	if (node->isLeaf()) {
		/* 
		* Rectangles are passed in with a 1 tile inset from the edge so that rooms 
		* will always be at least two tiles apart when they are generated later.
		*/
		Rect rm(node->x + 1, node->y + 1, node->w - 2, node->h - 2);
		map->rects.push_back(rm);
	}
	return true;
}



void GameMap::bsp_generate(int depth, int minh, int minv, float max_h_ratio, float max_v_ratio) {
	BSPCallBack* call_back = new BSPCallBack(this);
	bsp->splitRecursive(randomizer, depth, minh, minv, max_h_ratio, max_v_ratio);
	bsp->traversePreOrder(call_back, NULL);
	bsp_generate_rooms(MIN_ROOM_W, MIN_ROOM_H, MAX_ROOM_W, MAX_ROOM_H);

	// Dig rooms and tunnels
	for (int i = 0; i < rooms.size(); i++) {
		dig_room(rooms[i]);
	}
	
	int j = 1;
	for (int i = 0; i < rooms.size() - 1; i++) {
		dig_tunnel(rooms[i].center_x(), rooms[i].center_y(), rooms[j].center_x(), rooms[j].center_y());
		j++;
	}
	
	delete call_back;
}



void GameMap::bsp_generate_rooms(int minw, int minh, int maxw, int maxh) {
	Rect rct;
	Rect rm;
	for (int i = 0; i < rects.size(); i++) {
		rct = rects[i];
		// Skip this rectangle if a room cannot fit inside
		if (rct.w < minw || rct.h < minh) {
			continue;
		}
		// Trim down max dimensions if they exceed rectangle bounds
		if (rct.w < maxw) {
			maxw = rct.w;
		}
		if (rct.h - 2 < maxw) {
			maxh = rct.h;
		}
		// Choose room dimensions
		int width = randomizer->getInt(minw, maxw);
		int height = randomizer->getInt(minh, maxh);
		// Choose room position within rectangle
		int x = rct.x;
		int y = rct.y;
		x += randomizer->getInt(0, rct.w - width);
		y += randomizer->getInt(0, rct.h - height);
		// Create the Rect and add it to rooms list
		rm = Rect(x, y, width, height);
		rooms.push_back(rm);
	}
}



Rect GameMap::get_room(int index) {
	if (index < 0 || index > rooms.size() - 1) {
		throw std::invalid_argument("get_room(index) out of bounds.");
	}
	return rooms[index];
}




int GameMap::get_num_rooms() {
	return rooms.size();
}


void GameMap::dig(int x, int y) {
	Tile* t = GameMap::get_tile(x, y);
	t->set_transparent(true);
	t->set_passable(true);
	t->set_graphic(floor_graphic);
}



void GameMap::dig_room(int x, int y, int w, int h) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			dig(x + i, y + j);
		}
	}
}



void GameMap::dig_room(Rect rm) {
	dig_room(rm.x, rm.y, rm.w, rm.h);
}



void GameMap::dig_tunnel(int x1, int y1, int x2, int y2, bool dir) {
	if (dir) {
		int x_dif = x2 - x1;
		for (int i = 0; i != x_dif; i += (x_dif > 0) - (x_dif < 0)) {
			dig(x1 + i, y1);
		}
		int y_dif = y2 - y1;
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