#ifndef tactics_battle__ActionMove_h
#define tactics_battle__ActionMove_h

#include "../header.h"
#include "../Path.h"

namespace tactics { 
	namespace battle {

class ActionMove : public boardgame::Action
{
	REU__PROPERTY_READONLY(int, _unitIndex, UnitIndex);
	REU__PROPERTY_READONLY_PASS_REF(Path, _path, Path);

	ActionMove(int player, int index, 
			const Path& path) :
		boardgame::Action(player),
		_unitIndex(index),
		_path(path)
	{}

	virtual void print() // override
	{
		printf("Move: ");

		_path.print();
	}
};

}} // end of namespace tactics::battle

#endif // tactics_battle__ActionMove_h
