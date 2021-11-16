#include <iostream>
#include <libtcod.h>
#include <SDL.h>
#include "command.h"
#include "entity.h"
#include "game_map.h"
#include "input_handler.h"
#include "tile.h"

void render_entities(tcod::Console* console, std::vector<Entity*> entities);

int main(int argc, char* argv[]) {
    auto console = tcod::Console{ 80, 50 };  // Main console.
    // Configure the context.
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;  // This is required.
    params.console = console.get();  // Derive the window size from the console size.
    params.window_title = "The Great Roguelike";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;  // This allows some user-control of the context.
    params.argv = argv;

    auto tileset = tcod::load_tilesheet("assets/16x16-sb-ascii.png", { 16, 16 }, tcod::CHARMAP_CP437);
    params.tileset = tileset.get();

    TileGraphic wall{
        ' ',
        tcod::ColorRGB(TCOD_black),
        tcod::ColorRGB(TCOD_black),
        tcod::ColorRGB(TCOD_grey),
        tcod::ColorRGB(TCOD_grey),
    };

    TileGraphic floor{
        ' ',
        tcod::ColorRGB(TCOD_black),
        tcod::ColorRGB(TCOD_light_blue),
        tcod::ColorRGB(TCOD_blue),
        tcod::ColorRGB(TCOD_blue),
    };

    GameMap map(80, 50, wall, floor);
    map.generate_room(1, 1, 10, 10);

    std::vector<Entity*> entities(0);

    Entity player = Entity(2, 2, '@', tcod::ColorRGB(TCOD_white), "The Great Borealis");
    entities.push_back(&player);

    Entity goblin = Entity(5, 5, 'G', tcod::ColorRGB(TCOD_green), "Goblin");
    entities.push_back(&goblin);

    InputHandler input;

    auto context = tcod::new_context(params);

    while (1) {  // Game loop.
        TCOD_console_clear(console.get());
        render_entities(&console, entities);
        map.render_tiles(&console);
        context->present(console);  // Updates the visible display.
        Command* command = input.handle_input();
        if (command) {
            if (command->get_type() == CommandType::quit_command) {
                return 0;
            }
            if (command->get_type() == CommandType::move_command) {
                if (map.is_passable(player.getX() + command->getDx(), player.getY() + command->getDy())) {
                    command->execute(player);
                }
                else {
                    std::cout << "You walk into a wall.";
                }
            }
            // command->execute(player);
        }
        // context->convert_event_coordinates(event);  // Optional, converts pixel coordinates into tile coordinates.
    }
}

void render_entities(tcod::Console* c, std::vector<Entity*> entities) {
    Entity e;
    for (auto i = entities.begin(); i != entities.end(); i++) {
        e = **i;
        tcod::print(*c, { e.getX(), e.getY() }, std::string(1, e.getChar()), e.getColor(), std::nullopt);
    }
}