#ifndef BoardGame__Action_h_
#define BoardGame__Action_h_

// project header
#include "BoardGame.h"

namespace boardgame {

class Action {
	REU__PROPERTY(int, _player, Player);
public:

	Action(int player) :
		_player(player)
	{}
	
	virtual ~Action() {}

	virtual void print() 
	{
		printf("BoardGame::Action::print Override this method");
	}
};

} // end of namespace board

#endif // BoardGame__Action_h_
