#ifndef tactics_battle__State_h
#define tactics_battle__State_h

#include "header.h"
#include "Unit.h"
#include "Floor.h"

namespace tactics { 
	namespace battle {

class Action;

class State : public boardgame::State
{
	REU__PROPERTY(int, _width, Width);
	REU__PROPERTY(int, _height, Height);
	// DELETE unit pointer on destruct
	REU__PROPERTY_PASS_REF(std::vector<Unit*>, _listUnitp, ListUnitp);	
	REU__PROPERTY(std::vector<int>, _listWait0, ListWait0);
	REU__PROPERTY(std::vector<int>, _listWait1, ListWait1); // STACK
	REU__PROPERTY(std::vector<Floor>, _listFloor, ListFloor);

public:
	State(int playerMax, 
			int width, int height, const std::vector<Unit*>& listUnitp,
			const std::vector<Floor>& listFloor);

	State(const State& o);

	virtual ~State();

	/** Initialize wait list
	 *  @called State::State()
	 */
	bool init();

	/***** Getter ******/

	int getActiveUnitIndex() const
	{
		if (this->isTurnNormal()) {
			return _listWait0.at(0);
		}
		else if (this->isTurnWait()) {
			return _listWait1.at(0);
		}
		else {
			throw "Error: State::getActiveUnitIndex";
		}
	}

	const Unit& getActiveUnit() const
	{
		return *_listUnitp[this->getActiveUnitIndex()];
	}

	Unit* getActiveUnit_p()
	{
		return _listUnitp[this->getActiveUnitIndex()];
	}

	virtual int getPlayer() const //  override
	{
		return this->getActiveUnit().getOwner();
	}

	/***** Predicate ******/

	virtual bool isTurnNormal() const
	{
		return _listWait0.size() >= 1;
	}

	virtual bool isTurnWait() const
	{
		return _listWait0.size() == 0 && _listWait1.size() >= 1;
	}
	//
	

	/***** UPDATE State ******
	 *
	 * Called by make tree functions
	 */

	/** Simply pass turn. Called by attack melee, attack ranged, move ...
	 */
	virtual void UPDATE_nextUnit();

	/** Pass unit turn and push unit to wait1 list.
	 *  Can't call this method from wait turn.
	 */
	virtual void UPDATE_nextUnitWait();

	virtual void print(int depth = 0); // override
	virtual void printUnits(int depth = 0);

protected:
	virtual bool isWaitListNeedToInit() const
	{
		return _listWait0.size() == 0 && _listWait1.size() == 0;
	}
};

}} // end of namespace tactics::battle

#endif // tactics_battle__State_h

