#ifndef tactics_battle__Path_h
#define tactics_battle__Path_h

#include "header.h"

namespace tactics { 
	namespace battle {

class Path : public std::vector<boardgame::Point>
{
private:
	double _line; // = -1 . Caluculate later.

public:
	Path() :
		_line(-1.0)
   	{}

	virtual ~Path() {}

	/** Lazy eval
	 */
	double getLine()
	{
		if (_line == -1) {
			_line = this->countLine();
		}

		return _line;
	}

	void print()
	{
		printf("[%fL] ", this->getLine());

		for (auto point: *this) {
			point.print();
			printf(" -> ");
		}
	}

	static void printPaths(const std::vector<Path>& paths)
	{
		for (auto path: paths) {
			path.print();
			printf("\n");	
		}
	}

private:
	double countLine()
	{
		if (this->size() == 1) {
			return 0;	
		}

		int vx = this->at(1).getX() - this->at(0).getX();
		int vy = this->at(1).getY() - this->at(0).getY();

		this->_line = 1;
		this->countLine(vx, vy, 2);
	}

	double countLine(int vx, int vy, int index)
	{
		if (this->size() <= index) {
			return _line;
		}
		else {
			int vx2 = this->at(index).getX() - this->at(index - 1).getX();
			int vy2 = this->at(index).getY() - this->at(index - 1).getY();

			// vector is changed
			if (vx != vx2 || vy != vy2) {
				if (vx2 !=0 && vy2 != 0) {
					_line += 1.1; // XXX
				} else {
					_line += 1.0;
				}
			}

			this->countLine(vx2, vy2, index+1);
		}
	}
};

inline void printPaths(const std::vector<Path>& paths)
{
	for (auto path: paths) {
		path.print();
		printf("\n");	
	}
}


}} // end of namespace tactics::battle

#endif // tactics_battle__Path_h
