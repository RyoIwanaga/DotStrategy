#include "../Unit.h"
#include <cassert>

int main(void)
{
	tactics::battle::Unit unit1(1, 2, 3, 4 ,5 ,6, 7, 8, boardgame::Point(10, 10));
	unit1.print();printf("\n");
//
//	assert(unit1.isDead() == true);
//	assert(unit2.isDead() == false);
//	assert(unit1.isPosition(boardgame::Point(10, 10)) == false);
//	assert(unit1.isPosition(10, 10) == false);
//	assert(unit1.isPosition(boardgame::Point(0, 0)) == true);
//	assert(unit1.isPosition(0, 0) == true);
//	assert(unit1.isEnemy(unit2) == true);
//	assert(unit2.isEnemy(unit1) == true);
//	assert(unit1.isAttackable(unit2) == true);
//	assert(unit2.isAttackable(unit1) == false);
//
//	tactics::battle::Unit* unit1_p = new tactics::battle::Unit();
//	unit1_p->print();printf("\n");
//	*(unit1_p) = unit2; // COPY
//	unit1_p->print();printf("\n");


	return 0;
}

