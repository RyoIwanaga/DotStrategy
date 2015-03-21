#include "BoardGame.h"

namespace boardgame {

/******************
 * Class Method
 ******************/


/******************
 * Function
 ******************/

bool collectNeighborHex(std::set<Point>* result_p, const Point& point,
		int width, int height) {
	int x = point.getX();
	int y = point.getY();
	int minX = std::max(0, x - 1);
	int maxX = std::min(x + 1, width - 1);
	int minY = std::max(0, y - 1);
	int maxY = std::min(y + 1, height - 1);

	for (int yy = minY; yy <= maxY; yy++) {
		for (int xx = minX; xx <= maxX; xx++) {

			// same as given position
			if (x == xx && y == yy) {
				continue;	
			}
			else if (reu::isEven(y) && 
					xx == (x + 1) && (
						yy == (y + 1) ||
						yy == (y - 1))) 
			{
				continue;	
			}
			else if (reu::isOdd(y) && 
					xx == (x - 1) && (
						yy == (y + 1) ||
						yy == (y - 1))) 
			{
				continue;	
			}
			else {
				result_p->insert(Point(xx, yy));	
			}
		}
	}

	return true;	
}

bool collectNeighborCross(std::set<Point>* result_p, const Point& point,
		int width, int height)
{
	int x, y;

	x = point.getX() - 1; y = point.getY();
	if (0 <= x && 0 <= y && x < width && y < height)
		result_p->insert(Point(x, y));

	x = point.getX() + 1; y = point.getY();
	if (0 <= x && 0 <= y && x < width && y < height)
		result_p->insert(Point(x, y));

	x = point.getX(); y = point.getY() - 1;
	if (0 <= x && 0 <= y && x < width && y < height)
		result_p->insert(Point(x, y));

	x = point.getX(); y = point.getY() + 1;
	if (0 <= x && 0 <= y && x < width && y < height)
		result_p->insert(Point(x, y));
	
	return true;
}

bool collectNeighbor(std::set<Point>* result_p, 
		const Point& point, int width, int height,
		std::vector<Point>* listNegative_p)
{
	int x = point.getX();
	int y = point.getY();
	int xMin = std::max(0, x - 1);
	int yMin = std::max(0, y - 1);
	int xMax = std::min(width - 1, x + 1);
	int yMax = std::min(height - 1, y + 1);

	for (int yy = yMin; yy <= yMax; yy++) {
		for (int xx = xMin; xx <= xMax; xx++) {
			// Same point
			if (x == xx && y == yy) {
				continue;
			}

			// Empty negative list.
			if (listNegative_p == nullptr || 
					!(reu::vector::isMember(*listNegative_p, Point(xx, yy)))) 
			{
				result_p->insert(Point(xx, yy));
			} 
			else {
				continue;
			}
		}
	}
	
	return true;
}

void collectAreaPoints(std::set<Point>* result_p, const Point& point,
		int width, int height, int n)
{
	if (n == 0) {
		return;
	} else {
		std::set<Point> neighbors;
		collectNeighbor(&neighbors, point, width, height);
		collectNeighbor(result_p, point, width, height);

		// for neighbors
		for (auto p : neighbors) {
			collectAreaPoints(result_p, p, width, height,
					n - 1);
		}
	}
}

void printHexPoint(std::set<Point>* points_p, int width, int height) 
{
	printf("=== Board view ===\n");
	for (int y = 0; y < height; y++) {
		if (reu::isOdd(y))
			printf(" ");

		for (int x = 0; x < width; x++) {

			for (auto p: *points_p) {
				if (p.getX() == x && p.getY() == y) {
					printf("o");
					goto LOOP_END;
				}
			}

			// find any point
			printf("x");
LOOP_END:
			printf(" ");
		}

		printf("\n");
	}

	printf("=== Points ===\n");
	for (auto point : *points_p) {
		point.print();
		printf("\n");
	}
}

void printPoint(std::set<Point>* points_p, int width, int height)
{
	printf("=== Board view ===\n");
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			for (auto p: *points_p) {
				if (p.getX() == x && p.getY() == y) {
					printf("o");
					goto LOOP_END;
				}
			}

			// find any point
			printf("x");
LOOP_END:
			printf(" ");
		}
		printf("\n");
	}

	printf("=== Points ===\n");
	for (auto point : *points_p) {
		point.print();
		printf("\n");
	}
}
	
} // end of namespace boardgame

