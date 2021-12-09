#ifndef ENTITY_H_11_14_21
#define ENTITY_H_11_14_21

#include <libtcod.h>
#include "command.h"
#include "componet.h"
#include "game_map.h"


class Entity {
public:
	/*		Constructors + Destructor		*/	

	Entity();
	Entity(int x, int y, char c, tcod::ColorRGB clr, std::string n, bool blcks = true);

	/*				Componets				*/
	
	ComponetAi* ai = nullptr;
	ComponetCombatant* combatant = nullptr;
	ComponetDestructable* destructable = nullptr;

	/*			Standard methods			*/

	Entity* check_collision(GameMap* map);
	void execute_command(Command* command, GameMap* map);
	void update(GameMap* map, std::vector<Entity*>& entities);
	void move(int dx, int dy);
	void wait();

	/*				GetSet					*/

	std::string getName();
	void setName(std::string n);

	int getX();
	int getY();
	void setPos(int x, int y) { xpos = x; ypos = y; }

	char getChar();
	void setChar(char c);

	tcod::ColorRGB getColor();
	void setColor(tcod::ColorRGB color);

private:
	bool blocks;

	std::string name = "Entity";

	int xpos;
	int ypos;

	char chr;
	tcod::ColorRGB color = tcod::ColorRGB();
};

#endif