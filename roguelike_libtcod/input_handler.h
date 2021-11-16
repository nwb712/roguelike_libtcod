#ifndef INPUT_HANDLER_H_11_14_21
#define INPUT_HANDLER_H_11_14_21

#include <SDL.h>
#include "command.h"

class InputHandler {
public:
	Command* handle_input();
private:
	SDL_Event event;
	MoveCommand move;
	WaitCommand wait;
	QuitCommand quit;
};

#endif