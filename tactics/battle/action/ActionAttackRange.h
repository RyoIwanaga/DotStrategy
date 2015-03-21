#ifndef tactics_battle__ActionAttackRange_h
#define tactics_battle__ActionAttackRange_h

#include "../header.h"


namespace tactics { 
	namespace battle {

class ActionAttackRange : public boardgame::Action
{
	REU__PROPERTY(int, _unitIndex, UnitIndex);
	REU__PROPERTY(int, _targetIndex, TargetIndex);
	REU__PROPERTY(int, _damage, Damage);
	REU__PROPERTY(bool, _isTargetDead, IsTargetDead);
	REU__PROPERTY(boardgame::Point, _posFrom, PosFrom);
	REU__PROPERTY(boardgame::Point, _posTarget, PosTarget);
	REU__PROPERTY(int, _time, Time);
	REU__PROPERTY(int, _damage2, Damage2);

	ActionAttackRange(int player, 
			int unitIndex, int targetIndex,
			int damage,
			bool isTargetDead,
			const boardgame::Point posFrom, 
			const boardgame::Point posTarget,
			int time = 1,
			int damage2 = 0) :
		boardgame::Action(player),
		_unitIndex(unitIndex),
		_targetIndex(targetIndex),
		_damage(damage),
		_isTargetDead(isTargetDead),
		_posFrom(posFrom),
		_posTarget(posTarget),
		_time(time),
		_damage2(damage2)
	{}

	virtual void print() // override
	{
		printf("Attack::Range:: %d,%d -> %d,%d, %d damage",
				_posFrom.getX(),
				_posFrom.getY(),
				_posTarget.getX(),
				_posTarget.getY(),
				_damage);
	}
};

}} // end of namespace tactics::battle

#endif // tactics_battle__ActionAttackRange_h

