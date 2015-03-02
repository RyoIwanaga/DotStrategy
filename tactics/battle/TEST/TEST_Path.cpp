#include "../Battle.h"
#include <cassert>


int main(void)
{
	printf("TEST::Path\n");

	tactics::battle::Path path;
	path.push_back(boardgame::Point(1, 1));
	path.push_back(boardgame::Point(2, 1));
	path.push_back(boardgame::Point(3, 1));
	path.push_back(boardgame::Point(4, 2));

	path.print();

	return 0;
}
