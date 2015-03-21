#ifndef tactics_battle__Path_h
#define tactics_battle__Path_h

#include "header.h"

namespace tactics { 
	namespace battle {

class Path : public std::vector<boardgame::Point>
{
private:
	double _line; // = -1 . Caluculate this later.

public:
	Path() :
		_line(-1.0)
   	{}

	virtual ~Path() {}

	/** Lazy eval
	 */
	double forceLine()
	{
		if (_line == -1.0) {
			_line = this->countLine();
		}

		return _line;
	}

	void print()
	{
		printf("[%fL] ", forceLine());

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
		// no line
		if (this->size() == 1) {
			return 0.0;	
		}

		int vx = this->at(1).getX() - this->at(0).getX();
		int vy = this->at(1).getY() - this->at(0).getY();

		// init line
		if (vx !=0 && vy != 0) {
			_line = 1.001;
		} else {
			_line = 1.0;
		}

		// start counting
		this->countLine(vx, vy, 2);
	}

	double countLine(int vx, int vy, int index)
	{
		if (this->size() <= index) {
			return _line;
		}
		else {
			int vxNew = this->at(index).getX() - this->at(index - 1).getX();
			int vyNew = this->at(index).getY() - this->at(index - 1).getY();

			// vector is changed
			if (vx != vxNew || vy != vyNew) {
				if (vxNew !=0 && vyNew != 0) {
					_line += 1.001;
				} else {
					_line += 1.0;
				}
			}

			this->countLine(vxNew, vyNew, index+1);
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
