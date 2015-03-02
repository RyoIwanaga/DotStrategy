#include "../Battle.h"
#include <cassert>

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
	printf("TEST::Battle\n");
	// make units neeed To DELETE
	std::vector<tactics::battle::Unit*> units;
	units.push_back(new tactics::battle::Unit(1, 100, 50, 4 ,5, 3, 8, 0, 
				boardgame::Point(0, 0)));
	units.push_back(new tactics::battle::Unit(1, 100, 50, 4 ,5, 3, 8, 0, 
				boardgame::Point(0, 3)));
	units.push_back(new tactics::battle::UnitRanged(2, 100, 50, 4 ,5, 3, 10, 1, 
				boardgame::Point(3, 3)));
	units.push_back(new tactics::battle::UnitRanged(2, 100, 50, 4 ,5, 3, 11, 1, 
				boardgame::Point(1, 1)));

	// make state
	auto state_p = new tactics::battle::State(2, 9, 5, units);
//	state_p.print(2);

	// make game rule
	tactics::battle::Battle game;
	game.playerAddHuman();
	game.playerAddHuman();
//	game.playerAddAi(3);

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
