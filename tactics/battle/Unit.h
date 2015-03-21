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

	REU__PROPERTY(bool, _isUsedRetaliation, IsUsedRetaliation);

protected:
	const int DAMAGE_PERCENT = 20;

public:
//	static NEW_makeByName(const char* name)
//	{
//		sqlite3* db_p = NULL;
//		std::string fullpath = "test.db"; // FIXME
//		int err = 0;
//		sqlite3_stmt *pStmt = NULL;
//
//		sqlite3_close(db_p);
//
//		if (sqlite3_open(fullpath.c_str(), &db_p) == SQLITE_OK) {
//			// �f�[�^�̒��o
//			// �X�e�[�g�����g�̗p��
//			err = sqlite3_prepare_v2(db_p,
//					"SELECT * FROM unit where name = 'dog'", 64,
//					&pStmt, NULL);
//
//			if(err != SQLITE_OK){
//				printf("NG\n");
//			}
//			else{
//				// �f�[�^�̒��o
//				while(SQLITE_ROW == (err = sqlite3_step(pStmt)) ){
//					int id = sqlite3_column_int(pStmt, 0);
//					const unsigned char* name = sqlite3_column_text(pStmt, 1);
//					printf("id: %d, name: %s\n", id, name);
//				}
//				//
//				//			if(err != SQLITE_DONE){
//				//				/* TODO: �G���[���� */
//				//			}
//			}
//
//			// �X�e�[�g�����g�̉��
//			sqlite3_finalize(pStmt);
//		}
//
//		sqlite3_close(db_p);
//	}

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

	/** Unit deal 100 ~ 50 % damage depends on current health
	 *
	 *  100% hp	-> deal 100% damage
	 *  50% hp 	-> deal 75% damage
	 *  0% hp 	-> deal 50% damage
	 */
	int getDamageRandom() const
	{
		int damage = reu::RandMt::rangePercent(_damage, DAMAGE_PERCENT);
		int damageHarf = damage / 2;

		return damageHarf + damageHarf * getHpParsent() / 100;
	}

	virtual int getDamageMelee() const
	{
		return getDamageRandom();
	}

	virtual int getDamageRanged() const
	{
		throw "Exception: Unit::getDamageRanged";
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
		return false;
	}

	/** Unit was not retailed, he can counter attack.
	 */
	virtual bool isCanRetaliate()
	{
		// Dead unit can't attack
		if (this->isDead()) {
			return false;
		}

		return !_isUsedRetaliation;
	}

	virtual void UPDATE_retaliated()
	{
		_isUsedRetaliation = true;
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
