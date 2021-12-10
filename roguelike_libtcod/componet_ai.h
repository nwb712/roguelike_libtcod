#ifndef COMPONET_AI_H_12_3_21
#define COMPONET_AI_H_12_3_21

#include <SDL.h>
#include "command.h"
#include "game_map.h"

class Entity;

class ComponetAi {
public:
	virtual Command* update(GameMap* map, std::vector<Entity*> entities) = 0;
	Entity* check_collision(int x, int y, GameMap* map, std::vector<Entity*> entities);
private:
	Entity* self;
};


// This ai componet detirmines a command to be executed based on player input
class ComponetAiPlayer : public ComponetAi {
public:
	ComponetAiPlayer(Entity* s) {}
	Command* update(GameMap* map, std::vector<Entity*> entities);
private:

	Entity* self;
	
	MoveCommand move;
	WaitCommand wait;
	QuitCommand quit;

	SDL_Event event;
	Command* handle_input();
};



class ComponetAiEnemy : public ComponetAi {
public:
	Command* update(GameMap* map) { return new WaitCommand(); };
private:
};
#endif
