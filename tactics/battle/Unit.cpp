#include "Unit.h"

namespace tactics { 
	namespace battle {

Unit::Unit(int id, int hp, int damage, int attack, int defend, 
		int speed, int initiative, int owner, const boardgame::Point& p) :
	_id(id),
	_hp(hp),
	_hpMax(hp),
	_damage(damage),
	_attack(attack),
	_defend(defend),
	_speed(speed),
	_initiative(initiative),
	_owner(owner),
	_pos(p)
{}

Unit::Unit(const Unit& o) :
	_id(o.getId()),
	_hp(o.getHp()),
	_hpMax(o.getHpMax()),
	_damage(o.getDamage()),
	_attack(o.getAttack()),
	_defend(o.getDefend()),
	_speed(o.getSpeed()),
	_initiative(o.getInitiative()),
	_owner(o.getOwner()),
	_pos(o.getPos())
{}

void Unit::print()
{
	printf("[%d,%d](%d ID:%d %d/%d %d-%d-%d %d-%d", 
			_pos.getX(),
			_pos.getY(),
			_owner,
			_id,
			_hp,
			_hpMax,
			_damage,
			_attack,
			_defend,
			_speed,
			_initiative);
}

bool Unit::isNextToEnemy(const std::vector<Unit*>& units, 
		int width, int height)
{
	// for neighbor
	std::set<boardgame::Point> neighbors;
	boardgame::collectNeighbor(&neighbors, this->getPos(), width, height);
	for (auto neighbor : neighbors) {
		for (auto unit_p : units) {
			if (this->isAttackable(*unit_p) && unit_p->getPos() == neighbor) {
				return true;
			}
		}
	}

	return false;
}

}} // end of namespace tactics::battle

