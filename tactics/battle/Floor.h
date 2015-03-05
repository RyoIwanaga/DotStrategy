#ifndef tactics_battle__Floor_h
#define tactics_battle__Floor_h

#include "header.h"

namespace tactics { 
	namespace battle {

class Floor
{
	REU__PROPERTY(boardgame::Point, _pos, Pos);

	Floor(const boardgame::Point& p) :
		_pos(p)
	{}

	virtual ~Floor(){}

	bool isPassable()
	{
		return false;
	}

	void print()
	{
		_pos.print();
	}

	static bool collectRocks(std::vector<Floor>* result_p, 
			int width, int height, int max, int guard = 2);
};

}} // end of namespace tactics::battle

#endif // tactics_battle__Floor_h

