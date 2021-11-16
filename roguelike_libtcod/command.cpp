#include "command.h"
#include "entity.h"

// Command definitions
CommandType Command::get_type() {
	return command_type;
}


// MoveCommand definitions
CommandType MoveCommand::get_type() {
	return command_type;
}
MoveCommand::MoveCommand() {
	dx = 0; dy = 0;
}
MoveCommand::MoveCommand(int x, int y) {
	dx = x; dy = y;
}
void MoveCommand::execute(Entity& entity) {
	entity.move(dx, dy);
}
void MoveCommand::undo(Entity& entity) {
	entity.move(-dx, -dy);
}


// QuitCommand definitions
CommandType QuitCommand::get_type() {
	return command_type;
}
void QuitCommand::execute(Entity& entity) {
	return;
}
void QuitCommand::undo(Entity& entity) {
	return;
}



// WaitCommand definitions
CommandType WaitCommand::get_type() {
	return command_type;
}
void WaitCommand::execute(Entity& entity) {
	entity.wait();
}
void WaitCommand::undo(Entity& entity) {
	return; // Can't exactly unwait until time has been inplemented
}