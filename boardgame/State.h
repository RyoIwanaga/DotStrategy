#ifndef boardgame__State_h_
#define boardgame__State_h_

// project header
#include "BoardGame.h"

namespace boardgame {

class State 
{
	REU__PROPERTY(int, _player, Player);
	REU__PROPERTY(int, _playerMax, PlayerMax);

public:
	State(int player, int playerMax) :
		_player(player),
		_playerMax(playerMax)
	{}

	virtual ~State() {}

	void printDepth(int depth)
	{
		for (int i = 0; i < depth; i++)
			printf("_   ");
	}

	/*** virtual methods ***/

	virtual void print(int depth = 0) 
	{
		printf("boardgame::State::print depth=%d Override this method", depth);
	}
};

} // end of namespace boardgame

#endif // boardgame__State_h_
