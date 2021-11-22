#include <iostream>
#include <libtcod.h>
#include <SDL.h>
#include "constants.h"
#include "engine.h"
#include "entity.h"

Engine::Engine(int argc, char* argv[]) {
    /* Initialize Console and Context */
    console = tcod::Console{ DEF_WIDTH, DEF_HEIGHT };  // Main console.
    context = initialize_context(&console, argc, argv);

    /* Generate Map */



    /* Place Entities */
    player->setPos(map.get_room(0).center_x(), map.get_room(0).center_y());
    entities.push_back(player);

    Entity* goblin = new Entity(map.get_room(1).center_x(), 
        map.get_room(1).center_y(), 'G', tcod::ColorRGB(TCOD_green), "Goblin");
    entities.push_back(goblin);
}



Engine::~Engine() {
    delete_entities();
}



void Engine::update() {
    Command* command = input.handle_input();
    execute_player_command(command);
    if (command) {
        if (command->get_type() == CommandType::quit_command) {
            quit = true;
        }
    }
}



void Engine::execute_player_command(Command* command) {
    if (command) {
        if (command->get_type() == CommandType::quit_command) {
            quit = true;
        }
        if (command->get_type() == CommandType::move_command) {
            recompute_fov = true; // Need to recompute map fov after player move
            if (map.is_passable(player->getX() + command->getDx(), player->getY() + command->getDy())) {
                command->execute(*player);
            }
            else {
                // May be a good idea to create a message class
                std::cout << "You walk into a wall.\n";
            }
        }
    }
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
    if (recompute_fov) {
        map.compute_fov(player->getX(), player->getY());
        recompute_fov = false;
    }
    map.render_tiles(&console);
    render_entities();
    context->present(console);  // Updates the visible display.
}



void Engine::render_entities() {
    Entity* e;
    for (int i = 0; i < entities.size(); i++) {
        e = entities[i];
        if (map.is_in_fov(e->getX(), e->getY())){
            tcod::print(console, { e->getX(), e->getY() }, 
                std::string(1, e->getChar()), e->getColor(), std::nullopt);
        }
    }
};



void Engine::delete_entities() {
    for (int i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
    entities.clear();
}