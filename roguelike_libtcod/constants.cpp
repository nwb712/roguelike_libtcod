#include "constants.h"
#include "game_map.h"
#include "tile.h"


const int DEF_WIDTH = 80;
const int DEF_HEIGHT = 50;

const TileGraphic DEF_WALL_GRAPHIC{
        ' ',
        tcod::ColorRGB(TCOD_purple),
        tcod::ColorRGB(TCOD_purple),
        tcod::ColorRGB(TCOD_red),
        tcod::ColorRGB(TCOD_red),
};

const TileGraphic DEF_FLOOR_GRAPHIC{
    ' ',
    tcod::ColorRGB(TCOD_black),
    tcod::ColorRGB(TCOD_light_blue),
    tcod::ColorRGB(TCOD_blue),
    tcod::ColorRGB(TCOD_blue),
};

const BSPParams DEF_BSP_PARAMS{
    3 ,4 ,4 ,0.75 ,0.75
};