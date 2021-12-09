#ifndef COMPONET_AI_H_12_3_21
#define COMPONET_AI_H_12_3_21

#include <SDL.h>
#include "command.h"
#include "game_map.h"


class ComponetAi {
public:
	virtual void update(GameMap* map) = 0;
	Command* get_command() { return command; }
private:
	Command* command = nullptr;
};


// This ai componet detirmines a command to be executed based on player input
class ComponetAiPlayer : public ComponetAi {
public:
	void update(GameMap* map);
private:
	void handle_input();

	Command* command = nullptr;
	SDL_Event event;
	MoveCommand move;
	WaitCommand wait;
	QuitCommand quit;
};
#endif
