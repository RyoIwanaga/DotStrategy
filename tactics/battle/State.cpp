#include "Battle.h"

namespace tactics { 
	namespace battle {

State::State(int playerMax, 
		int width, int height, 
		const std::vector<Unit*>& listUnitp,
		const std::vector<Floor>& listFloor) :
	boardgame::State(-1, playerMax),
	_width(width),
	_height(height),
	_listFloor(listFloor)
{
	for (auto unit_p : listUnitp) {
		_listUnitp.push_back(unit_p->clone());
	}

	// Initialize rest conditions
	this->init();
}

State::State(const State& o) :
	boardgame::State(-1, o.getPlayerMax()),
	_width(o.getWidth()),
	_height(o.getHeight()),
	_listWait0(o.getListWait0()),
	_listWait1(o.getListWait1()),
	_listFloor(o.getListFloor())
{
	for (auto unit_p : o.getListUnitp()) {
		_listUnitp.push_back(unit_p->clone());
	}
}

State::~State()
{
	for (auto unit_p : _listUnitp) {
		delete unit_p;
	}
}

bool State::init()
{
	_listWait0.clear();
	_listWait1.clear();

	// reset retaliation counter
	for (Unit* unit_p : _listUnitp) {
		unit_p->setIsUsedRetaliation(false);
	}

	for (unsigned int i=0; i<_listUnitp.size(); i++) {
		// only use living unit
		if (!(_listUnitp.at(i)->isDead()))
			// add index
			_listWait0.push_back(i);
	}

	int swap;
	for (unsigned int i=0; i<_listWait0.size()-1; i++) {
		for (unsigned int j=i+1; j<_listWait0.size(); j++) {
			// swap
			if (_listUnitp.at(_listWait0[i])->getInitiative() < 
					_listUnitp.at(_listWait0[j])->getInitiative()) {

				swap = _listWait0[i];
				_listWait0[i] = _listWait0[j];
				_listWait0[j] = swap;
			}
		}
	}

	return true;
}

void State::UPDATE_nextUnit()
{
	// When normal turn
	if (this->isTurnNormal()) {
		// remove first unit index from normal list
		_listWait0.erase(_listWait0.begin());

		if (this->isWaitListNeedToInit()) {
			this->init();
			return;
		} 
		else if (this->getActiveUnit().isDead()) {
			this->UPDATE_nextUnit();
			return;	
		} 
		else {
			return;
		}
	}
	// When wait turn
	else if (this->isTurnWait()){
		// remove first unit index from wait list
		_listWait1.erase(_listWait1.begin());

		if (this->isWaitListNeedToInit()) {
			this->init();
			return;
		} 
		else if (this->getActiveUnit().isDead()) {
			this->UPDATE_nextUnit();
			return;	
		} 
		else {
			return;
		}
	}
	else {
		throw "Error: State::UPDATE_nextUnit";
	}
}

void State::UPDATE_nextUnitWait()
{
	// Can't call this method from wait turn
	assert(this->isTurnNormal() == true);

	_listWait1.insert(_listWait1.begin(), _listWait0[0]);
	this->UPDATE_nextUnit();	
}

void State::print(int depth)
{

	int n;

	this->printUnits(depth);


	/***** Wait0 *****/

	n = 0;
	this->printDepth(depth);
	printf("=== Wait0 ===\n");
	for (auto i : _listWait0) {
		this->printDepth(depth);
		printf("[%d]:", i);
		_listUnitp[i]->print();
		printf("\n");

		n++;
	}

	/***** Wait1 *****/

	n = 0;
	this->printDepth(depth);
	printf("=== Wait1 ===\n");
	for (auto i : _listWait1) {
		this->printDepth(depth);
		printf("[%d]:", i);
		_listUnitp[i]->print();
		printf("\n");

		n++;
	}

	/***** Board *****/

	this->printDepth(depth);
	printf("=== Board ===\n");
	for (int y = 0; y < _height * 2; y++) {
		this->printDepth(depth);

		for (int x = 0; x < _width; x++) {

			// for floor
			for (auto floor : _listFloor) {
				if (floor.getPos() == boardgame::Point(x, y / 2)) {

					if (reu::isEven(y)) {
						printf("####");
					} else {
						printf("ROCK");
					}

					goto LOOP_END;
				}
			}

			// for units
			for (auto unit_p : _listUnitp) {
				if (unit_p->getPos() == boardgame::Point(x, y / 2)) {

					if (reu::isEven(y)) {
						printf("%d:%2d",
								unit_p->getOwner(),
								unit_p->getId());
					} else {
						printf("%4d",
								unit_p->getHp());
					}

					goto LOOP_END;
				}
			}

			if (reu::isEven(y)) {
				printf(" .  ");
			} else {
				printf("    ");
			}
LOOP_END:
			printf(" ");

		}
		printf("\n");
	}
}

void State::printUnits(int depth)
{

	int n;

	n = 0;
	this->printDepth(depth);
	printf("=== Units ===\n");
	for (auto unit_p : _listUnitp) {
		this->printDepth(depth);
		printf("[%d]:", n);
		unit_p->print();
		printf("\n");

		n++;
	}
}

}} // end of namespace tactics::battle

