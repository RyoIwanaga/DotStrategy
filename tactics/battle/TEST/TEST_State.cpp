#include "../State.h"
#include <cassert>

int main(void)
{
	// make units neeed To DELETE
	std::vector<tactics::battle::Unit*> units;
	units.push_back(new tactics::battle::Unit(1, 2, 3, 4 ,5, 3, 8, 0, 
				boardgame::Point(0, 0)));
	units.push_back(new tactics::battle::Unit(1, 2, 3, 4 ,5, 3, 10, 1, 
				boardgame::Point(3, 3)));

	tactics::battle::State state(2, 9, 5, units);
	state.print(2);

	assert(state.getActiveUnitIndex() == 1);
	state.getActiveUnit_p()->print();
	assert(state.getPlayer() == 1);


	

	return 0;
}
