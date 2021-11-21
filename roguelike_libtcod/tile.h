#ifndef TILE_H_11_15_21
#define TILE_H_11_15_21

#include <libtcod.h>
#include "constants.h"

struct TileGraphic {
	char c = ' ';
	tcod::ColorRGB color_fg = tcod::ColorRGB(TCOD_black);
	tcod::ColorRGB color_bg = tcod::ColorRGB(TCOD_light_blue);
	tcod::ColorRGB color_dark_fg = tcod::ColorRGB(TCOD_purple);
	tcod::ColorRGB color_dark_bg = tcod::ColorRGB(TCOD_blue);
};

struct TileData {
	bool passable = false;
	bool transparent = false;
	bool dark = true;
};

class Tile {
public:
	Tile() { graphic = TileGraphic(); }
	Tile(TileGraphic g) {graphic = g;}
	Tile(TileGraphic g, TileData d) { graphic = g; data = d; }


	TileGraphic get_graphic() { return graphic; }
	void set_graphic(TileGraphic g) { graphic = g; }
	TileData get_data() { return data; }
	void set_data(TileData d) { data = d; }


	bool is_passable() { return data.passable; };
	void set_passable(bool pass) { data.passable = pass; }
	void toggle_passable() { data.passable = !data.passable; };

	bool is_transparent() { return data.transparent; };
	void set_transparent(bool tran) { data.transparent = tran; }
	void toggle_transparent() { data.transparent = !data.transparent; };

	bool is_dark() { return data.dark; };
	void set_dark(bool dark) { data.dark = dark; }
	void toggle_dark() { data.dark = !data.dark; };
private:
	TileGraphic graphic;
	TileData data;
};

#endif
