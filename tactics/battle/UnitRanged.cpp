#include "UnitRanged.h"

namespace tactics { 
	namespace battle {

UnitRanged::UnitRanged(int id, int hp, int damage, int attack, int defend, 
		int speed, int initiative, int owner, const boardgame::Point& p) :
	Unit(id, hp, damage, attack, defend,
			speed, initiative, owner, p)
{}

UnitRanged::UnitRanged(const UnitRanged& o) :
	Unit(o)
{}

}} // end of namespace tactics::battle

