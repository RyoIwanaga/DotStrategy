#include "Battle.h"
#include "action/actions.h"
#include "Unit.h"
#include "UnitRanged.h"

namespace tactics { 
	namespace battle {

std::vector<boardgame::Tree<State>*>* 
Battle::makeChilds(boardgame::Tree<State>* tree_p, int flag)
{
	auto result_p = new std::vector<boardgame::Tree<State>*>();
	auto state_p = tree_p->getState_p();

	auto player = state_p->getPlayer();
	auto width = state_p->getWidth();
	auto height = state_p->getHeight();
	auto activeUnitIndex = state_p->getActiveUnitIndex();
	auto activeUnit_p = state_p->getActiveUnit_p();
	auto units = state_p->getListUnitp();

	// ranged and not surrounded
	bool isShootable = activeUnit_p->isRanged() &&
			// blocked by enemy?
			!(activeUnit_p->isNextToEnemy(units, width, height));

	REU__PRINT(":mc" << flag);

	/***** Game End ? *****/

	std::vector<int> wins;
	collectWinner(&wins, *state_p);
	if (wins.size() >= 1) {
		return result_p;
	}


	/***** Collect Move : AttackRanged *****/

	if (isShootable) {
		collectAttackRanged(result_p, *state_p);
	}

	/***** Collect path *****/

	std::vector<boardgame::Point> negativePoints;
	// collect unit points as negative list
	collectUnitPos(&negativePoints, units);
	// collect floor points as negative list
	for (auto floor : state_p->getListFloor()) {
		negativePoints.push_back(floor.getPos());
	}

	// collect!!
	std::vector<Path> paths;
	collectPath(&paths, activeUnit_p->getPos(),
			width, height, 
			activeUnit_p->getSpeed(),
			&negativePoints);

	/***** Collect Move : AttackMelee *****/

	if (!isShootable) {
		collectMoveAttackMelee(result_p, *state_p, paths);
	}

	// Ai and already made attack child
	if (flag == this->FORCE_KIND_AI && result_p->size() >= 1) {
		return result_p; // RETURN
	}

	/***** PUSH wait action when normal turn *****/

	if (state_p->isTurnNormal()) {
		result_p->push_back(NEW_TreeWait(*state_p));
	}

	/***** Collect Move : Move *****/

	for (auto path : paths) {
		// make Action
		auto action_p = new tactics::battle::ActionMove(
				player, activeUnitIndex,
				path);

		// make state
		auto newState_p = new tactics::battle::State(*state_p);
		auto newUnits = newState_p->getListUnitp();

		// move active unit
		newUnits.at(activeUnitIndex)->setPos(path.back());

		// go next turn
		newState_p->UPDATE_nextUnit();

		result_p->push_back(new boardgame::Tree<State>(
			action_p, newState_p));
	}

	return result_p;
}

bool Battle::collectWinner(std::vector<int>* wins_p, const State& state)
{
	if (reu::vector::every<Unit*>(state.getListUnitp(),
				[](Unit* unit_p)
				{
					if (unit_p->getOwner() != 1) {
						return true;	
					} 
					return unit_p->isDead();
				})) {
		wins_p->push_back(0);
	}

	if (reu::vector::every<Unit*>(state.getListUnitp(),
				[](Unit* unit_p)
				{
					if (unit_p->getOwner() != 0) {
						return true;	
					} 
					return unit_p->isDead();
				})) 
	{
		wins_p->push_back(1);
	}

	return true;
}

int Battle::scoreState(const State& state, int player)
{
	int score = 0;

	for (auto unit_p : state.getListUnitp()) {
		if (unit_p->getOwner() == player) {
			score += unit_p->getHp();
		} else {
			score -= unit_p->getHp();
		}
	}
	
	return score;
}

/***** Function ******/

/*** Collect move ***/

bool collectAttackRanged(std::vector<boardgame::Tree<State>*>* result_p, 
		const State &state)
{
	auto units = state.getListUnitp();

	for (unsigned int i; i < units.size(); i++) {
		if (state.getActiveUnit().isAttackable(*units[i])) {
			result_p->push_back(
					NEW_TreeAttackRanged(state, i));
		}
	}
}

bool collectMoveAttackMelee(std::vector<boardgame::Tree<State>*>* result_p, 
		const State& state, 
		const std::vector<Path>& paths)
{
	// collect stay attack
	Path path0;
	path0.push_back(state.getActiveUnit().getPos());
	collectMoveAttackMeleeFromPath(result_p, state, path0);

	// collect move attack
	for (auto path : paths) {
		collectMoveAttackMeleeFromPath(result_p, state, path);
	}

	return true;
}

bool collectMoveAttackMeleeFromPath(std::vector<boardgame::Tree<State>*>* result_p, 
		const State& state, const Path& path)
{
	auto activeUnitIndex = state.getActiveUnitIndex();
	auto activeUnit = state.getActiveUnit();

	// collect neighbor
	std::set<boardgame::Point> neighbors;
	boardgame::collectNeighbor(&neighbors, path.back(),
			state.getWidth(), state.getHeight());

	for (auto neighbor : neighbors) {
		// find index of attackable unit
		int targetIndex = reu::vector::positionIf<Unit*>(state.getListUnitp(), 
				[&neighbor, &activeUnit] (Unit* unit_p)
				{
				return unit_p->getPos() == neighbor 
				&& activeUnit.isAttackable(*unit_p);
				});

		// find it
		if (targetIndex >= 0) {
			result_p->push_back(NEW_TreeAttackMelee( // NEW
						state, path, targetIndex, neighbor));
		}
	}

	return true;
}

/*** Make new tree ***/

boardgame::Tree<State>* 
NEW_TreeAttackRanged(const State& state, int targetIndex)
{
	auto activeUnitIndex = state.getActiveUnitIndex();

	// make state
	auto newState_p = new tactics::battle::State(state);
	auto newUnits = newState_p->getListUnitp();

	// shoot
	int damage = state.getActiveUnit().getDamageRanged();
	bool isDead = newUnits.at(activeUnitIndex)->UPDATE_attack(
			newUnits.at(targetIndex), damage);

	// make action
	auto action_p = new tactics::battle::ActionAttackRange(
			state.getPlayer(), activeUnitIndex, targetIndex, 
			damage, isDead,
			newUnits[activeUnitIndex]->getPos(),
			newUnits[targetIndex]->getPos());

	// go next turn
	newState_p->UPDATE_nextUnit();

	return new boardgame::Tree<State>(
				action_p, newState_p);
}


/****** Make NEW tree ******/

boardgame::Tree<State>* 
NEW_TreeWait(const State& state)
{
	auto activeUnitIndex = state.getActiveUnitIndex();

	// copy state
	auto newState_p = new tactics::battle::State(state);
	auto newUnits = newState_p->getListUnitp();
	Unit* newUnitActive = newUnits.at(activeUnitIndex);

	// make action
	auto action_p = new tactics::battle::ActionWait(
			state.getPlayer(), activeUnitIndex);

	// go next turn
	newState_p->UPDATE_nextUnitWait();

	return new boardgame::Tree<State>(
				action_p, newState_p);
}

boardgame::Tree<State>* 
NEW_TreeAttackMelee(const State& state, const Path& path,
		int targetIndex, const boardgame::Point pTarget)
{
	auto activeUnitIndex = state.getActiveUnitIndex();

	// make new state
	auto newState_p = new tactics::battle::State(state);
	auto newUnits = newState_p->getListUnitp();
	Unit* newUnitActive = newUnits.at(activeUnitIndex);
	Unit* newUnitTarget = newUnits.at(targetIndex);

	/***** UPDATE new state *****/

	/*** Move ***/

	newUnitActive->setPos(path.back());

	/*** attack ***/

	std::vector<Attack> attacks;

	/* first attack */

	// first attack
	int damage1 = newUnitActive->getDamageMelee();
	// UPDATE
	bool isDead1 = newUnitActive->UPDATE_attack(newUnitTarget, damage1);
	// PUSH result
	attacks.push_back(Attack(activeUnitIndex, damage1, isDead1));


	/* second(conter) attack */

	if (newUnitTarget->isCanRetaliate()) {
		int damage2 = newUnitTarget->getDamageMelee();
		// UPDATE
		bool isDead2 = newUnitTarget->UPDATE_attack(newUnitActive, damage2);
		// PUSH result
		attacks.push_back(Attack(targetIndex, damage2, isDead2));

		newUnitTarget->UPDATE_retaliated();
	}
	else {
		attacks.push_back(Attack()); // make no damage attack
	}

	// make action
	auto action_p = new tactics::battle::ActionAttackMelee(
			state.getPlayer(), activeUnitIndex, targetIndex, 
			path, pTarget, attacks);

	// go next turn
	newState_p->UPDATE_nextUnit();

	return new boardgame::Tree<State>(
				action_p, newState_p);
}


bool collectUnitPos(std::vector<boardgame::Point>* result_p, 
		const std::vector<Unit*>& units)
{
	for (auto unit_p : units) {
		if (!unit_p->isDead())
			result_p->push_back(unit_p->getPos());
	}

	return true;
}

void collectPath(std::vector<Path>* result_p, 
		const boardgame::Point& point,
		int width, int height, int range,
		std::vector<boardgame::Point>* listNegative_p, 	// = nullptr
		Path* previous) 								// = nullptr
{
	std::vector<Path> newPaths;

	// collect neighbors
	std::set<boardgame::Point> neighbors;
	boardgame::collectNeighbor(&neighbors, point,
			width, height, listNegative_p);

	// When Paths is empty(No history) make new path
	if (result_p->empty()) {

		for (auto neighbor : neighbors) {

			// make new path (length = 1)
			Path path;
			path.push_back(point);
			path.push_back(neighbor);

			// append this path
			result_p->push_back(path);
			newPaths.push_back(path);
		}
	}
	// With previous path
	else {
		for (auto neighbor : neighbors) {
			// make new path from previous path
			Path path = *previous;
			path.push_back(neighbor);

			// append this path 
			int index = reu::vector::positionIf<Path>(
					*result_p, 
					[&neighbor]
					(const Path& path)
					{
						return neighbor == path.back();
					});
			if (index < 0)
			{
				result_p->push_back(path);
			}
			// Found
			else {
				// If find shorter path, swap
				if (result_p->at(index).size() > path.size()) {
					result_p->at(index) = path;
				}
				// Same size but less line
				else if (result_p->at(index).size() == path.size() &&
						result_p->at(index).forceLine() > path.forceLine()) {
					result_p->at(index) = path;
				}
			}
			newPaths.push_back(path);
		}
	}

	if (range > 1) {
		for (auto path : newPaths) {
			collectPath(result_p, path.back(),
					width, height,
					range - 1,
					listNegative_p,
					&path); // pass this path
		}
	}
}

}} // end of namespace tactics::battle

