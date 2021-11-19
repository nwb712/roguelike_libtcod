#include <iostream>
#include <libtcod.h>
#include <SDL.h>
#include "command.h"
#include "constants.h"
#include "entity.h"
#include "game_map.h"
#include "input_handler.h"
#include "tile.h"

/*
* TODO: Separate main game loop into an engine class which is initialized here. 
* This class should manage all entities, the map, and the rendering of those two.
*/

tcod::ContextPtr initialize_context(tcod::Console* console, int argc, char* argv[]);
void render_entities(tcod::Console* console, std::vector<Entity*> entities);
bool execute_player_command(Command* command, GameMap* map, Entity* player);

int main(int argc, char* argv[]) {
    int console_w = 80;
    int console_h = 50;

    auto console = tcod::Console{ DEF_WIDTH, DEF_HEIGHT };  // Main console.
    auto context = initialize_context(&console, argc, argv);

    GameMap map(DEF_WIDTH, DEF_HEIGHT, DEF_WALL_GRAPHIC, 
        DEF_FLOOR_GRAPHIC, DEF_BSP_PARAMS);

    map.dig_room(1, 1, 10, 10);
    map.dig_tunnel(20, 20, 5, 5, true);

    std::vector<Entity*> entities(0);

    Entity player = Entity(2, 2, '@', tcod::ColorRGB(TCOD_white), "The Great Borealis");
    entities.push_back(&player);

    Entity goblin = Entity(5, 5, 'G', tcod::ColorRGB(TCOD_green), "Goblin");
    entities.push_back(&goblin);

    InputHandler input;

    while (1) {  // Game loop.
        TCOD_console_clear(console.get());
        render_entities(&console, entities);
        map.render_tiles(&console);
        context->present(console);  // Updates the visible display.
        Command* command = input.handle_input();
        execute_player_command(command, &map, &player);
        if (command) {
            if (command->get_type() == CommandType::quit_command) {
                return true;
            }
        }
    }
}



bool execute_player_command(Command* command, GameMap* map, Entity* player) {
    if (command) {
        if (command->get_type() == CommandType::quit_command) {
            return false;
        }
        if (command->get_type() == CommandType::move_command) {
            if (map->is_passable(player->getX() + command->getDx(), player->getY() + command->getDy())) {
                command->execute(*player);
            }
            else {
                // May be a good idea to create a message class
                std::cout << "You walk into a wall.\n";
            }
        }
    }
    return true;
}



tcod::ContextPtr initialize_context(tcod::Console* console,int argc, char* argv[]) {
    // Configure the context.
    auto params = TCOD_ContextParams{};

    params.tcod_version = TCOD_COMPILEDVERSION;  // This is required.
    params.console = console->get();  // Derive the window size from the console size.
    params.window_title = "The Great Roguelike";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;  // This allows some user-control of the context.
    params.argv = argv;

    auto tileset = tcod::load_tilesheet("assets/16x16-sb-ascii.png", { 16, 16 }, tcod::CHARMAP_CP437);
    params.tileset = tileset.get();
    return tcod::new_context(params);
}



void render_entities(tcod::Console* c, std::vector<Entity*> entities) {
    Entity e;
    for (auto i = entities.begin(); i != entities.end(); i++) {
        e = **i;
        tcod::print(*c, { e.getX(), e.getY() }, std::string(1, e.getChar()), e.getColor(), std::nullopt);
    }
}