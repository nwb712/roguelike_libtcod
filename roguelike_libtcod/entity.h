#ifndef ENTITY_H_11_14_21
#define ENTITY_H_11_14_21

#include <libtcod.h>



class Entity {
public:
	Entity();
	Entity(int x, int y, char c, tcod::ColorRGB clr, std::string n);

	void move(int dx, int dy);
	void wait();

	std::string getName();
	void setName(std::string n);

	int getX();
	int getY();

	char getChar();
	void setChar(char c);

	tcod::ColorRGB getColor();
	void setColor(tcod::ColorRGB color);

private:
	std::string name = "Entity";
	int xpos;
	int ypos;
	char chr;
	tcod::ColorRGB color = tcod::ColorRGB();
};

#endif