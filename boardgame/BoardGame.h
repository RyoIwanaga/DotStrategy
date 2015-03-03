#ifndef boardgame__BoardGame_h_
#define boardgame__BoardGame_h_

// my utility
#include "../reu/reu.h"

// c lib
#include <stdio.h>

// cpp lib
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>

// my header
#include "Point.h"
#include "State.h"
#include "Action.h"
#include "Tree.h"
#include "Ai.h"

namespace boardgame {

/** Game controller class.
 */
template <class T> // State type
class BoardGame 
{
	// DELETE elements on destruct
	REU__PROPERTY_READONLY_PASS_REF(std::vector<Ai<T>*>, _listAi_p, ListAi_p);

public:
	BoardGame() {}
	virtual ~BoardGame()
	{
		for (auto ai_p : _listAi_p) {
			delete ai_p;
		}
	}

	virtual void play(Tree<T>* tree_p);

	/** Make child branches from current state. Please OVERRIDE
	 *  @called boardgame::Tree::force()
	 */
	virtual std::vector<Tree<T>*>* 
	makeChilds(Tree<T>* tree_p)
	{
		return new std::vector<Tree<T>*>();
	}

	/** Collect winners. plese OVERRIDE
	 *  @called super class
	 *  @called Ai::play, Ai::choice
	 */
	virtual bool collectWinner(std::vector<int>* wins_p, const T& state)
	{ 
		return false;
	}

	/** Score tree.
	 */
	virtual int scoreTree(Tree<T>& tree, int player);

	void playerAddAi(int level)
	{
		this->_listAi_p.push_back(new Ai<T>(level, *this));
	}

	void playerAddHuman()
	{
		this->_listAi_p.push_back(nullptr);
	}

	void playerChangeToAi(int n, int level)
	{
		delete this->_listAi_p[n];
		this->_listAi_p[n] = new Ai<T>(level, *this);
	}

	void playerChangeToHuman(int n)
	{
		delete this->_listAi_p[n];
		this->_listAi_p[n] = nullptr;
	}

protected:
	/** Return tree, player choose.
	*/
	virtual Tree<T>* hundleHuman(Tree<T>* tree_p);

	/** Return tree pointer as Ai
	 */
	virtual Tree<T>* hundleAi(Tree<T>* tree_p, Ai<T>& ai);

	/** Score game end tree.
	 */
	virtual int scoreTreeGameEnd(Tree<T>& tree, int player);

	/** Score game state (not game end). Please OVERRIDE
	 */
	virtual int scoreState(const T& state, int player) 
	{
		return 123;	
	}
};

template <class T>	
void BoardGame<T>::play(Tree<T>* tree_p)
{
	auto state_p = tree_p->getState_p();
	auto player = state_p->getPlayer();

	printf("\n===============================================\n");

//	REU_DEBUG__SENTENCE(bool, isKilled, 
//			tree_p->DELETE_askParentToDeleteBrother());

	tree_p->print();
	tree_p->force();

	//	std::vector<int> winPlayers;
	//	dod::collectWinner(&winPlayers, tree_p->getState_p());

	tree_p->force();
	// game end
	if (tree_p->isTerminal()) {
		std::vector<int> wins;
		// use override function
		this->collectWinner(&wins, *(tree_p->getState_p()));
		for (auto i: wins)
			printf("win: %d\n", i);
	}
	// player
	else if (_listAi_p[player] == nullptr) {
		play(hundleHuman(tree_p));
	} else {
		play(hundleAi(tree_p, *(_listAi_p[player])));
	}
}

template <class T>
Tree<T>* BoardGame<T>::hundleHuman(Tree<T>* tree_p)
{
	auto state_p = tree_p->getState_p();

	printf("\n=== Hundle player %d as Human ===\n", 
			state_p->getPlayer());
	printf("Choice your move:\n");

	// print choice
	auto i = 1;
	for (auto treeNext_p: *(tree_p->force())) {
		auto action_p = treeNext_p->getAction_p();

		printf("%2d: ", i);
		action_p->print();
		printf("\n");

		i++;
	}

	int n;
	printf("\n > ");
	std::cin >> n;
	printf("\n");

	return tree_p->force()->at(n - 1);
}

template <class T>
Tree<T>* BoardGame<T>::hundleAi(Tree<T>* tree_p, Ai<T>& ai)
{
	auto state_p = tree_p->getState_p();
	auto player = state_p->getPlayer();

	// Choice heir child
	auto heirIndex = ai.choice(tree_p, player);
	auto heir_p = tree_p->force()->at(heirIndex);

	auto scores = ai.getPreScores();

	// Print
	printf("\n=== Hundle player %d as AI ===\n", 
			state_p->getPlayer());

	auto i = 0;
	for (auto treeNext_p: *(tree_p->force())) {
		if (i == heirIndex) {
			printf("[*]");
		} else {
			printf("[ ]");
		}

		auto action_p = treeNext_p->getAction_p();
		action_p->print();

		printf("[%d]\n", scores[i]);

		i++;
	}
	REU__NEWLINE;

	return heir_p;
}

template <class T>
int BoardGame<T>::scoreTree(Tree<T>& tree, int player)
{
	// Score game end
	if (tree.isForced()) {
		return this->scoreTreeGameEnd(tree, player);
	}
	// Score state	
	else {
		return this->scoreState(*(tree.getState_p()), player);
	}
}

template <class T>
int BoardGame<T>::scoreTreeGameEnd(Tree<T>& tree, int player)
{
	// Collect winners
	std::vector<int> wins;
	this->collectWinner(&wins, *(tree.getState_p()));

	if (reu::vector::isMember(wins, player)) {
		return INT_MAX - 10 / wins.size();
	} 
	else {
		return INT_MIN + 10;
	}
}

template <class T>
int scoreState(T& state, int player)
{
	return 123;
}

/******************
 * Function
 ******************/

/**	x o o x
 * 	 o @ o x
 * 	x o o x
 * 	 x x x x
 */
bool collectNeighborHex(std::set<Point>* result_p, const Point& point,
		int width, int height);

/** ..o.
 *  .oxo
 *  ..o.
 */
bool collectNeighborCross(std::set<Point>* result_p, const Point& point,
		int width, int height);

/** .ooo
 *  .oxo
 *  .ooo
 */
bool collectNeighbor(std::set<Point>* result_p, 
		const Point& point, int width, int height,
		std::vector<Point>* listNegative_p = nullptr);

/** xoo.
 *  ooo.
 *  ooo.
 */
void collectAreaPoints(std::set<Point>* result_p, const Point& point,
		int width, int height, int n);

void printHexPoint(std::set<Point>* points_p, int width, int height);
void printPoint(std::set<Point>* points_p, int width, int height);

} // end of namespace boardgame

#endif // boardgame__BoardGame_h_
