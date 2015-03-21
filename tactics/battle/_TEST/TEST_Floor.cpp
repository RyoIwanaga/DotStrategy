#include "../Battle.h"
#include <cassert>

namespace tb = tactics::battle;

int main(void)
{
	printf("TEST::Floor\n");

	std::vector<tb::Floor> floors;
	tb::Floor::collectRocks(&floors,
			9, 5, 10);

	for (auto floor : floors) {
		floor.print();
		printf("\n");
	}

	return 0;
}
