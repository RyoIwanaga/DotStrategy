#ifndef boardgame__Ai_h_
#define boardgame__Ai_h_

// project header
#include "BoardGame.h"

namespace boardgame {

template <class T> // State
class BoardGame;

template <class T> // State
class Ai 
{
	// Limited depth ai read
	REU__PROPERTY_READONLY(int, _level, Level);
	REU__PROPERTY_READONLY(int, _kind, Kind);
	REU__PROPERTY_READONLY(std::vector<int>, _preScores, PreScores);

public:
	static const int KIND_DEFAULT = 1;

	Ai(int level, BoardGame<T>& rule, int kind = KIND_DEFAULT) :
		_level(level),
		_kind(kind),
		_rule(rule) 
	{}

	virtual ~Ai() {}

	virtual int choice(Tree<T>* tree_p, int player);

private:
	BoardGame<T>& _rule;

	virtual bool collectScores(std::vector<int>* scores_p, Tree<T>& tree, int player);
	virtual int getScore(Tree<T>& tree, int player);
};

template <class T> // State
int Ai<T>::choice(Tree<T>* tree_p, int player)
{
	// FORCE tree
	tree_p->forceRec(_level, _kind);

	// Collect child scores
	_preScores.clear();
	this->collectScores(&_preScores, *tree_p, player);

	// Choice heir child
	auto max = reu::vector::max(_preScores);

	return reu::vector::position(_preScores, max);
}

template <class T> // State
bool Ai<T>::collectScores(std::vector<int>* scores_p, Tree<T>& tree, int player)
{
	// for children
	for (auto child_p: *(tree.force(_kind))) {
		scores_p->push_back(getScore(*child_p, player));
	}

	return true;
}

template <class T> // State
int Ai<T>::getScore(Tree<T>& tree, int player)
{
	// When terminal calculate this state
	if(tree.isTerminal()) {
		return _rule.scoreTree(tree, player);
	} 
	else {
		std::vector<int> scores;
		this->collectScores(&scores, tree, player);

		if (tree.getState_p()->getPlayer() == player) {
			return reu::vector::max(scores);
		} 
		else {
			return reu::vector::min(scores);
		}
	}
}

} // end of namespace boardgame

#endif // boardgame__Ai_h_
