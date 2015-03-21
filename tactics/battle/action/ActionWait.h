#ifndef tactics_battle__ActionWait_h
#define tactics_battle__ActionWait_h

#include "../header.h"

namespace tactics { 
	namespace battle {

class ActionWait : public boardgame::Action
{
	REU__PROPERTY_READONLY(int, _unitIndex, UnitIndex);

	ActionWait(int player, int index) :
		boardgame::Action(player),
		_unitIndex(index)
	{}

	virtual void print() // override
	{
		printf("Wait.");
	}
};

}} // end of namespace tactics::battle

#endif // tactics_battle__ActionWait_h
