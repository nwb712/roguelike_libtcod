#ifndef COMMAND_H_11_14_21
#define COMMAND_H_11_14_21

#include <iostream>
#include "entity.h"

enum class CommandType {
	command,
	move_command,
	quit_command,
	wait_command,
};

class Command {
public:
	virtual ~Command() {}
	virtual CommandType get_type();
	virtual void execute(Entity& entity) = 0;
	virtual void undo(Entity& entity) = 0;
	virtual int getDx() = 0;
	virtual int getDy() = 0;
private:
	CommandType command_type = CommandType::command;
};

class MoveCommand : public Command {
public:
	MoveCommand();
	MoveCommand(int x, int y);
	virtual CommandType get_type();
	virtual void execute(Entity& entity);
	virtual void undo(Entity& entity);
	virtual int getDx() { return dx; }
	virtual int getDy() { return dy; }
private:
	CommandType command_type = CommandType::move_command;
	int dx;
	int dy;
};

class QuitCommand : public Command {
public:
	virtual CommandType get_type();
	virtual void execute(Entity& entity);
	virtual void undo(Entity& entity);
	virtual int getDx() { return 0; }
	virtual int getDy() { return 0; }
private:
	CommandType command_type = CommandType::quit_command;
};

class WaitCommand : public Command {
public:
	virtual CommandType get_type();
	virtual void execute(Entity& entity);
	virtual void undo(Entity& entity);
	virtual int getDx() { return 0; }
	virtual int getDy() { return 0; }
private:
	CommandType command_type = CommandType::wait_command;
};

#endif