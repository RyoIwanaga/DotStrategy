#include "../State.h"
#include <cassert>

namespace tb = tactics::battle;

int main(void)
{
	// make units
	std::vector<tb::Unit*> units;
	units.push_back(new tb::Unit(1, 100, 50, 4 ,5, 3, 8, 0, 
				boardgame::Point(0, 0)));
	units.push_back(new tb::Unit(1, 100, 50, 4 ,5, 3, 8, 1, 
				boardgame::Point(1, 0)));

	// make stone
	std::vector<tb::Floor> stones;
	tb::Floor::collectRocks(&stones, 9, 5, 3);

	tactics::battle::State state(2, 9, 5, units, stones);
	state.print(2);
//
//	assert(state.getActiveUnitIndex() == 1);
//	state.getActiveUnit_p()->print();
//	assert(state.getPlayer() == 1);


	

	return 0;
}
