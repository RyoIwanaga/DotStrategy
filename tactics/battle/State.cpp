#include "Battle.h"

namespace tactics { 
	namespace battle {

State::State(int playerMax, 
		int width, int height, const std::vector<Unit*>& listUnitp) :
	boardgame::State(-1, playerMax),
	_width(width),
	_height(height)
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
	_listWait1(o.getListWait1())
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

	// only use living unit
	for (unsigned int i=0; i<_listUnitp.size(); i++) {
		// is not dead
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

void State::UPDATE_nextUnit() // FIXME wait1 ���l�����Ă��Ȃ�
{
	if (_listWait0.size() == 1) {
		_listWait0.clear();
		// remake wait list
		this->init();

		return;
	} 
	else {
		// remove first index (current index of unit)
		_listWait0.erase(_listWait0.begin());

		// XXX ��Ԉُ���l�����Ă��Ȃ�
		if (_listUnitp[_listWait0[0]]->isDead()) {
			this->UPDATE_nextUnit();			
		} 
		else {
			// DONE
			return;
		}
	}
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
