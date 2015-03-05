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
	REU__PROPERTY(std::vector<int>, _listWait1, ListWait1);
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

	int getActiveUnitIndex() const
	{
		if (!(_listWait0.empty())) {
			return _listWait0.at(0);
		}
		else if (!(_listWait1.empty())) {
			return _listWait1.at(0);
		}
		else {
			throw "State::getActiveUnitIdex error";
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

	// TODO Unit test when dead
	virtual void UPDATE_nextUnit();

	virtual void print(int depth = 0); // override
	virtual void printUnits(int depth = 0);
};

}} // end of namespace tactics::battle

#endif // tactics_battle__State_h

