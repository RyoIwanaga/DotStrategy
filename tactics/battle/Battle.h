#ifndef tactics_battle__Battle_h
#define tactics_battle__Battle_h

#include "header.h"
#include "Unit.h"
#include "UnitRanged.h"
#include "State.h"
#include "Path.h"
#include "action/actions.h"

namespace tactics { 
	namespace battle {

class Battle : public boardgame::BoardGame<State>
{
public:
	Battle() {}
	virtual ~Battle() {}

protected:
	// OVERRIDE
	virtual std::vector<boardgame::Tree<State>*>* 
		makeChilds(boardgame::Tree<State>* tree_p); 						
	virtual bool collectWinner(std::vector<int>* wins_p, const State& state);
	virtual int scoreState(const State& state, int player);
};

/***** Function *****/

/*** Collect move ***/


bool collectAttackRanged(std::vector<boardgame::Tree<State>*>* result_p, 
		const State &state);

bool collectMoveAttackMelee(std::vector<boardgame::Tree<State>*>* result_p, 
		const State& state, 
		const std::vector<Path>& paths);

bool collectMoveAttackMeleeFromPath(std::vector<boardgame::Tree<State>*>* result_p, 
		const State& state, const Path& path);

/*** Make new tree ***/

boardgame::Tree<State>* 
NEW_TreeAttackRanged(const State& state, int targetIndex);

boardgame::Tree<State>* 
NEW_TreeAttackMelee(const State& state, const Path& path,
		int targetIndex, const boardgame::Point pTarget);


/** Collect unit positions
 */
bool collectUnitPos(std::vector<boardgame::Point>* result_p, 
		const std::vector<Unit*>& units);

/** Collect Path
 */
void collectPath(std::vector<Path>* result_p, 
		const boardgame::Point& point,
		int width, int height, int range,
		std::vector<boardgame::Point>* listNegative_p = nullptr,
		Path* previous = nullptr);

}} // end of namespace tactics::battle

#endif // tactics_battle__Battle_h
