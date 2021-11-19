#include <iostream>
#include <libtcod.h>
#include "input_handler.h"
#include "command.h"

Command* InputHandler::handle_input() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
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
}