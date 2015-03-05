#ifndef tactics_battle__UnitRanged_h
#define tactics_battle__UnitRanged_h

#include "header.h"
#include "Unit.h"

namespace tactics { 
	namespace battle {

class UnitRanged : public Unit
{
public:
	UnitRanged(int id, int hp, int damage, int attack, int defend, 
			int speed, int initiative, int owner, const boardgame::Point& p);


	UnitRanged(const UnitRanged& o);

	virtual UnitRanged* clone() const 
	{
		return new UnitRanged(*this);	
	}

	virtual ~UnitRanged() {}

	/***** Getter *****/

	virtual int getDamageMelee() const
	{
		return Unit::getDamageMelee() / 2;
	}

	virtual int getDamageRanged() const
	{
		return getDamageRandom();
	}

	/***** Predicate *****/

	virtual bool isRanged() const
	{
		return true;
	}


	virtual void print()
	{
		Unit::print();

		printf(" archer ");
	}

};

}} // end of namespace tactics::battle

#endif // tactics_battle__UnitRanged_h
