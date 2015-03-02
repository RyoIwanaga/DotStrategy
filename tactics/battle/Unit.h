#ifndef tactics_battle__Unit_h
#define tactics_battle__Unit_h

#include "header.h"


namespace tactics { 
	namespace battle {

class Unit
{
	REU__PROPERTY(int, _id, Id);
	REU__PROPERTY(int, _hp, Hp);
	REU__PROPERTY(int, _hpMax, HpMax);
	REU__PROPERTY(int, _damage, Damage);
	REU__PROPERTY(int, _attack, Attack);
	REU__PROPERTY(int, _defend, Defend);
	REU__PROPERTY(int, _speed, Speed);
	REU__PROPERTY(int, _initiative, Initiative);
	REU__PROPERTY(int, _owner, Owner);
	REU__PROPERTY(boardgame::Point, _pos, Pos);

public: 
	Unit(int id, int hp, int damage, int attack, int defend, 
			int speed, int initiative, int owner, const boardgame::Point& p);

	Unit(const Unit& o);

	virtual Unit* clone() const 
	{
		return new Unit(*this);	
	}

	virtual ~Unit() {}


	/***** Selector ******/

	virtual int getHpParsent() const
	{
		return (double) _hp / _hpMax * 100;
	}

	virtual int getDamageMelee() const
	{
		REU_DEBUG__PRINT("unit");
		return _damage;
	}

	/***** Predicate ******/

	virtual bool isDead() const
	{
		return _hp <= 0;
	}

	virtual bool isPosition(const boardgame::Point& p) const
	{
		return _pos == p;
	}

	virtual bool isPosition(int x, int y) const
	{
		return this->isPosition(boardgame::Point(x, y));
	}

	virtual bool isEnemy(const Unit& u) const
	{
		return _owner != u.getOwner();
	}

	virtual bool isAttackable(const Unit& u) const
	{
		return !(u.isDead()) && this->isEnemy(u);
	}

	virtual bool isNextToEnemy(const std::vector<Unit*>& units,
			int width, int height);

	virtual bool isRanged() const
	{
		REU_DEBUG__PRINT("unit::isranged");
		return false;
	}

	virtual bool UPDATE_attack(Unit* unitp, int damage)
	{
		int hp = std::max(0, unitp->getHp() - damage);

		unitp->setHp(hp);

		return unitp->isDead();
	}

	virtual void print();
};

}} // end of namespace tactics::battle

#endif // tactics_battle__Unit_h
