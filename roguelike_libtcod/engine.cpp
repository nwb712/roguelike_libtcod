#include <iostream>
#include <libtcod.h>
#include <SDL.h>
#include "command.h"
#include "constants.h"
#include "engine.h"
#include "entity.h"
#include "entity_list.h"
#include "game_map.h"


Engine::Engine(int argc, char* argv[]) {

    console = tcod::Console{ DEF_WIDTH, DEF_HEIGHT };  // Main console.
    context = initialize_context(&console, argc, argv);

    map = new GameMap(DEF_WIDTH, DEF_HEIGHT, DEF_WALL_GRAPHIC, DEF_FLOOR_GRAPHIC, DEF_BSP_PARAMS);

    entities = new EntityList(map, player);

    // Set player position and spawn enemies
    player = new Entity(map->get_room(0).center_x(), map->get_room(0).center_y(),
        '@', tcod::ColorRGB(TCOD_white), "The Great Borealis");
    player->ai = new ComponetAiPlayer(player);
    entities->add_entity(player);

    populate_enemies(MAX_ROOM_POP);
}



Engine::~Engine() {
    // Delete any dynamically allocated resources here
    entities->clear_entities();
    delete entities;

    delete map;
}



void Engine::update() {
    Command* command;
    if (map->recompute_fov) {
        map->compute_fov(player->getX(), player->getY());
    }
    switch (game_state) {
    case STARTUP:
        game_state = IDLE;
        break;
    case IDLE:
        player->update(map, entities->entities);
        game_state = NEW_TURN;
        break;
    case NEW_TURN:
        entities->update();
        game_state = IDLE;
        break;
    case VICTORY:
        std::cout << "You win \n";
        quit = true;
        break;
    case DEFEAT:
        std::cout << "You lose \n";
        quit = true;
        break;
    }
}



void Engine::add_enemy(int x, int y) {
    // In the future a random stat block and enemy type will be picked here
    Entity* e = new Entity(x, y, 'G', tcod::ColorRGB(TCOD_green), "Goblin");
    entities->add_entity(e);
}



void Engine::populate_enemies(int max_pop) {
    TCODRandom* r = new TCODRandom;
    Entity* e;

    Rect room;
    int num_enemies;

    int xpos;
    int ypos;

    for (int i = 0; i < map->get_num_rooms(); i++) {
        num_enemies = r->getInt(1, max_pop);
        room = map->get_room(i);
        while (num_enemies > 0) {
            xpos = r->getInt(room.x, room.x + room.w - 1);
            ypos = r->getInt(room.y, room.y + room.h - 1);
            e = entities->check_collision_at(xpos, ypos);
            if (!e) {
                add_enemy(xpos, ypos);
                num_enemies--;
            }
        }
    }
    delete r; // Dynamically allocated randomizer needs to be deleted
}



tcod::ContextPtr Engine::initialize_context(tcod::Console* console, int argc, char* argv[]) {
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



void Engine::render() {
    TCOD_console_clear(console.get());
    map->render_tiles(&console);
    entities->render(&console);
    context->present(console);  // Updates the visible display.
}