#include "constants.h"
#include "game_map.h"
#include "tile.h"


const int DEF_WIDTH = 80;
const int DEF_HEIGHT = 50;

const int MIN_ROOM_W = 5;
const int MAX_ROOM_W = 8;
const int MIN_ROOM_H = 5;
const int MAX_ROOM_H = 8;

const int MAX_ROOM_POP = 3;

const TileGraphic DEF_WALL_GRAPHIC{
        '#',
        tcod::ColorRGB(0, 255, 255),
        tcod::ColorRGB(70, 70, 70),
        tcod::ColorRGB(0, 125, 125),
        tcod::ColorRGB(0, 0, 0),
};

const TileGraphic DEF_FLOOR_GRAPHIC{
    ' ',
    tcod::ColorRGB(100, 100, 100),
    tcod::ColorRGB(175, 175, 120),
    tcod::ColorRGB(50, 50, 50),
    tcod::ColorRGB(85, 85, 60),
};

const BSPParams DEF_BSP_PARAMS{
    6 ,7 ,7 ,1.0 ,1.0
};