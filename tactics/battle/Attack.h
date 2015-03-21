#ifndef tactics_battle__Attack_h
#define tactics_battle__Attack_h

#include "header.h"

namespace tactics {
	namespace battle {

class Attack
{
	REU__PROPERTY_READONLY(int, _indexUnit, IndexUnit);
	REU__PROPERTY_READONLY(int, _damage, Damage);
	REU__PROPERTY_READONLY(bool, _isDead, IsDead);

	Attack(int indexUnit, int damage, bool isDead = false) :
		_indexUnit(indexUnit),
		_damage(damage),
		_isDead(isDead)
	{}

	// make no retaliation
	Attack() :
		_indexUnit(-1),
		_damage(-1),
		_isDead(false)
	{}

	bool isHaveDamage() const
	{
		return _damage >= 0;
	}

	virtual ~Attack() {}

	void print()
	{
		if (this->isHaveDamage()) {
			std::cout << _damage << " damage.";
			if (_isDead) {
				printf("[dead] ");
			}
			else {
				printf(" ");
			}
		}
		else {
			printf("No retaliation.");
		}
	}
};

}} // end of namespace tactics::battle

#endif // tactics_battle__Attack_h
