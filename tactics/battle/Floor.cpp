#include "Floor.h"

namespace tactics { 
	namespace battle {

bool Floor::collectRocks(std::vector<Floor>* result_p, 
		int width, int height, int max, int guard) // = 2
{
	// guard first unit positions
	int minX = guard;
	int maxX = width - guard - 1;

	std::set<boardgame::Point> points;

	// collect point 
	do {
		points.insert(boardgame::Point(
					reu::RandMt::range(maxX, minX),
					reu::RandMt::range(height - 1)));
	} while (points.size() < max);

	// make rock
	for(auto p : points) {
		result_p->push_back(Floor(p));
	}
	
	return true;	
}

}} // end of namespace tactics::battle

