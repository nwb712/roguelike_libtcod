#include <libtcod.h>
#include <SDL.h>
#include "componet_ai.h"
#include "entity.h"



Entity* ComponetAi::check_collision(int x, int y, GameMap* map, std::vector<Entity*> entities) {
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->getX() == x && entities[i]->getY() == y) {
            return entities[i];
        }
    }
    return nullptr;
}



Command* ComponetAiPlayer::update(GameMap* map, std::vector<Entity*> entities) {
    Command* command = handle_input();
    /* Trying to move collision logic into ai componet. It is not going well...
    if ((command) && command->get_type() == CommandType::move_command) {
        Entity* other = check_collision(self->getX() + command->getDx(), 
            self->getY() + command->getDy(), map, entities);

        if (other) {
            if (other->destructable) {
                ;
            }
        }
    }
    */
    return command;
}



Command* ComponetAiPlayer::handle_input(){
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quit = QuitCommand();
            return &quit;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_KP_1:
                move = MoveCommand(-1, 1);
                return &move;
                break;
            case SDL_SCANCODE_KP_2:
                move = MoveCommand(0, 1);
                return &move;
                break;
            case SDL_SCANCODE_KP_3:
                move = MoveCommand(1, 1);
                return &move;
                break;
            case SDL_SCANCODE_KP_4:
                move = MoveCommand(-1, 0);
                return &move;
                break;
            case SDL_SCANCODE_KP_5:
                wait = WaitCommand();
                return &wait;
                break;
            case SDL_SCANCODE_KP_6:
                move = MoveCommand(1, 0);
                return &move;
                break;
            case SDL_SCANCODE_KP_7:
                move = MoveCommand(-1, -1);
                return &move;
                break;
            case SDL_SCANCODE_KP_8:
                move = MoveCommand(0, -1);
                return &move;
                break;
            case SDL_SCANCODE_KP_9:
                move = MoveCommand(1, -1);
                return &move;
                break;
            }
            break;
        }
    }
    return nullptr;
}