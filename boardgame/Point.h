#ifndef boardgame__Point_h_
#define boardgame__Point_h_

// project header
#include "BoardGame.h"

namespace boardgame {

class Point {
	REU__PROPERTY(int, _x, X);
	REU__PROPERTY(int, _y, Y);

public:
	Point(int x, int y) :
		_x(x),
		_y(y)
	{}
	
	virtual ~Point() {}

	void set(int x, int y)
	{
		_x = x;
		_y = y;
	}

	void print() const
	{
		printf("%d,%d",
				_x, _y);
	}

	int getHash() const
	{
		return _x * 1000 + _y; // XXX
	}



	friend bool operator <(const Point& o1, const Point& o2); // for std::set
	friend bool operator ==(const Point& o1, const Point& o2);
};

inline bool operator <(const Point& o1, const Point& o2)
{
	return o1.getHash() < o2.getHash();
}

inline bool operator ==(const Point& o1, const Point& o2)
{
	return o1.getHash() == o2.getHash();
}

} // end of namespace boardgame

#endif // boardgame__Point_h_
