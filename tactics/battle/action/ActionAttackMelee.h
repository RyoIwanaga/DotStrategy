#ifndef tactics_battle__ActionAttackMelee_h
#define tactics_battle__ActionAttackMelee_h

#include "../header.h"
#include "../Path.h"

namespace tactics { 
	namespace battle {

class ActionAttackMelee : public boardgame::Action
{
	REU__PROPERTY_READONLY(int, _unitIndex, UnitIndex);
	REU__PROPERTY_READONLY(int, _targetIndex, TargetIndex);
	REU__PROPERTY_READONLY(int, _damage, Damage);
	REU__PROPERTY_READONLY(bool, _isTargetDead, IsTargetDead);
	REU__PROPERTY_READONLY_PASS_REF(Path, _path, Path);
	REU__PROPERTY_READONLY_PASS_REF(boardgame::Point, _posTarget, PosTarget);

	ActionAttackMelee(int player, 
			int unitIndex, int targetIndex,
			int damage,
			const Path& path, 
			const boardgame::Point& posTarget,
			bool isTargetDead = false) :
		boardgame::Action(player),
		_unitIndex(unitIndex),
		_targetIndex(targetIndex),
		_damage(damage),
		_isTargetDead(isTargetDead),
		_path(path),
		_posTarget(posTarget)
	{}

	virtual void print() // override
	{
		printf("Attack::Melee:: Move ");
		_path.print();

		printf("Then attack %d,%d damage %d.",
				_posTarget.getX(),
				_posTarget.getY(),
				_damage);
	}
};

}} // end of namespace tactics::battle

#endif // tactics_battle__ActionAttackMelee_h

