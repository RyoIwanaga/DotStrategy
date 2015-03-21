#include "../Battle.h"
#include <cassert>


int main(void)
{
	printf("TEST::Path\n");
	{
		tactics::battle::Path path;
		path.push_back(boardgame::Point(7, 1));
		path.push_back(boardgame::Point(6, 2)); // + 1.0001
		path.push_back(boardgame::Point(5, 2)); // + 1
		assert(path.forceLine() == 2.001);
	}

	{
		tactics::battle::Path path;
		path.push_back(boardgame::Point(7, 1));
		path.push_back(boardgame::Point(6, 2)); // + 1.0001
		path.push_back(boardgame::Point(5, 2)); // + 1
		path.push_back(boardgame::Point(4, 2));
		assert(path.forceLine() == 2.001);
	}

	{
		tactics::battle::Path path;
		path.push_back(boardgame::Point(7, 1));
		path.push_back(boardgame::Point(6, 2)); // + 1.0001
		path.push_back(boardgame::Point(5, 3));
		assert(path.forceLine() == 1.001);
	}

//	path.push_back(boardgame::Point(4, 2));


	return 0;
}
