#include <iostream>
#include <libtcod.h>
#include <SDL.h>
#include "command.h"
#include "constants.h"
#include "engine.h"
#include "entity.h"
#include "game_map.h"
#include "input_handler.h"
#include "tile.h"



int main(int argc, char* argv[]) {
    Engine engine = Engine(argc, argv);
    while (!engine.quit) {
        engine.update();
        engine.render();
    }
    return true;
    
}