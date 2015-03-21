#include "../Battle.h"
#include <cassert>

namespace tb = tactics::battle;

void TEST_collectPath()
{
	printf("TEST::Battle::collectPath\n");

	std::vector<tactics::battle::Path> paths;
	tactics::battle::collectPath(
			&paths, 
			boardgame::Point(5, 5),
			10, 10, 3);

	tactics::battle::Path::printPaths(paths);
}

void TEST_play()
{
	int width = 9;
	int height = 5;

	printf("TEST::Battle\n");
	// make stone
	std::vector<tactics::battle::Unit*> units;
	units.push_back(new tactics::battle::Unit(1, 100, 50, 4 ,5, 3, 10, 0,
				boardgame::Point(1, 0)));
	units.push_back(new tactics::battle::Unit(1, 100, 50, 4 ,5, 3, 10, 0,
				boardgame::Point(1, 3)));
	units.push_back(new tactics::battle::Unit(1, 100, 50, 4 ,5, 3, 8, 1,
				boardgame::Point(7, 3)));
//	units.push_back(new tactics::battle::UnitRanged(2, 200, 50, 4 ,5, 3, 8, 1,
//				boardgame::Point(7, 3)));
//	units.push_back(new tactics::battle::UnitRanged(2, 200, 50, 4 ,5, 3, 8, 1,
//				boardgame::Point(7, 1)));

	// make stone
	std::vector<tb::Floor> stones;
	tb::Floor::collectRocks(&stones, width, height, 4);

	// make state
	auto state_p = new tactics::battle::State(2, width, height, units, stones);
//	state_p.print(2);

	// make game rule
	tactics::battle::Battle game;
	game.playerAddAi(5);
	game.playerAddHuman();
//	game.playerAddHuman();

	auto tree_p = new boardgame::Tree<tactics::battle::State>(
			state_p, &game); 
//	tree_p->force();
//	tree_p->printRec(1);

	game.play(tree_p);
}

int main(void)
{
	printf("TEST::Battle\n");

//	TEST_collectPath();
	TEST_play();



	return 0;
}
