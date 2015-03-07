#ifndef tactics_battle__ActionAttackMelee_h
#define tactics_battle__ActionAttackMelee_h

#include "../header.h"
#include "../Attack.h"
#include "../Path.h"

namespace tactics { 
	namespace battle {

class ActionAttackMelee : public boardgame::Action
{
	REU__PROPERTY_READONLY(int, _unitIndex, UnitIndex);
	REU__PROPERTY_READONLY(int, _targetIndex, TargetIndex);
	REU__PROPERTY_READONLY_PASS_REF(Path, _path, Path);
	REU__PROPERTY_READONLY_PASS_REF(boardgame::Point, _posTarget, PosTarget);
	// size is 2 or higher
	REU__PROPERTY_READONLY_PASS_REF(std::vector<Attack>, _attacks, Attacks);

	ActionAttackMelee(int player, 
			int unitIndex,
			int targetIndex,
			const Path& path, 
			const boardgame::Point& posTarget,
			const std::vector<Attack>& attacks) :
		boardgame::Action(player),
		_unitIndex(unitIndex),
		_targetIndex(targetIndex),
		_path(path),
		_posTarget(posTarget),
		_attacks(attacks)
	{
		// first attack must have damage
		assert(_attacks[0].isHaveDamage() == true);

		// at least contain 2 attack.
		assert(_attacks.size() >= 2);

		// if enemy unit is dead he can't attack
		if (_attacks[0].getIsDead()) {
			assert(!(_attacks[1].isHaveDamage()));
		}

		// if enemy unit is dead he can't attack

		// double attack rule
		if (_attacks.size() == 3) {
			// double attacker is can not die.
			assert(_attacks[1].getIsDead() == false);
			// second attack must have damage;
			assert(_attacks[2].isHaveDamage() == true);
		}

	}

	bool isRetaliated()
	{
		return false; // todo
	}

	virtual void print() // override
	{
		printf("Attack::Melee:: Move ");
		_path.print();

		printf(" enemy (%d,%d). \n\t",
				_posTarget.getX(),
				_posTarget.getY());

		for (auto attack : _attacks) {
			attack.print();
		}
	}
};

}} // end of namespace tactics::battle

#endif // tactics_battle__ActionAttackMelee_h

