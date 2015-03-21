#ifndef boardgame__Tree_h_
#define boardgame__Tree_h_

// project header
#include "BoardGame.h"

namespace boardgame {

template <class T>
class BoardGame;

template <class T>
class Tree 
{
	REU__PROPERTY(Action*, _action_p, Action_p);			// DELETE on destruct
	REU__PROPERTY(T*, _state_p, State_p);					// DELETE on destruct

	// Use these member for get detail.
	// ex. attack
	REU__PROPERTY(Action*, _actionDetail_p, ActionDetail_p);// DELETE on destruct
	REU__PROPERTY(T*, _stateDetail_p, StateDetail_p);		// DELETE on destruct

	REU__PROPERTY(Tree<T>*, _parent_p, Parent_p);		// POINTER

protected:
	// child pointers 
	std::vector<Tree<T>*>* _vector_child_pointers_p;	// DELETE on destruct
	// heir
	Tree<T>* _heir_p;									// POINTER
	// for makechilds method
	BoardGame<T>* _rule_p;								// POINTER

public:
	/** Root node
	 */
	Tree(T* state_p, BoardGame<T>* rule_p) :
		_action_p(nullptr),
		_state_p(state_p),
		_actionDetail_p(nullptr),
		_stateDetail_p(nullptr),
		_parent_p(nullptr),
		_heir_p(nullptr),
		_vector_child_pointers_p(nullptr),
		_rule_p(rule_p)
	{}

	/** Normal node
	 */
	Tree(Action* action_p, T* state_p) :
		_action_p(action_p),
		_state_p(state_p),
		_actionDetail_p(nullptr),
		_stateDetail_p(nullptr),
		_parent_p(nullptr),
		_heir_p(nullptr),
		_vector_child_pointers_p(nullptr),
		_rule_p(nullptr) 
	{}

	/** Have normal action/state and detail action/state
	 */
	Tree(Action* action_p, T* state_p, Action* actionD_p, T* stateD_p) :
		_action_p(action_p),
		_state_p(state_p),
		_actionDetail_p(actionD_p),
		_stateDetail_p(stateD_p),
		_parent_p(nullptr),
		_heir_p(nullptr),
		_vector_child_pointers_p(nullptr),
		_rule_p(nullptr)
	{}

	/** Don't delete these pointers
	 * _parent_p
	 * _heir_p
	 * _rule_p
	 */
	virtual ~Tree() 
	{
		delete _action_p;
		delete _state_p;
		delete _actionDetail_p;
		delete _stateDetail_p;

		if (_vector_child_pointers_p != nullptr) {
			for (auto child_p: *_vector_child_pointers_p) {
				delete child_p;	
			}
			delete _vector_child_pointers_p;
		}
	}

	/*** Predicate ***/

	bool isForced()
	{
		return _vector_child_pointers_p != nullptr;
	}

	bool isTerminal()
	{
		return !this->isForced() || 
			_vector_child_pointers_p->size() == 0;
	}

	bool isDetailNode()
	{
		return _stateDetail_p != nullptr;
	}

	/*** Method ***/

	bool DELETE_children()
	{
		if (_vector_child_pointers_p != nullptr) {
			for (auto child_p: *_vector_child_pointers_p)
			{
				delete child_p;
			}
		}
		// DELETE vector
		delete _vector_child_pointers_p;
		_vector_child_pointers_p = nullptr;

		return true;
	}

	bool DELETE_childrenExceptHeir(Tree<T>* heir_p)
	{
		int indexHeir = reu::vector::position<Tree<T>*>(*_vector_child_pointers_p, heir_p);

		if (indexHeir <= -1) {
			return false;
		}

		// Move heir pointer
		_heir_p = heir_p;

		auto i = 0;
		// DELETE all child except heir
		for (auto child_p: *_vector_child_pointers_p)
		{
			if (i != indexHeir) {
				REU_DEBUG__PRINT("DELETE index" << i);
				delete child_p;	
			}
			i++;
		}
		// DELETE vector
		delete _vector_child_pointers_p;

		return true;
	}

	/** Ask parent to kill his children except heir
	 */
	bool DELETE_askParentToDeleteBrother()
	{
		if (_parent_p == nullptr) {
			return false;
		}
		else {
			return _parent_p->DELETE_childrenExceptHeir(this);
		}
	}

	std::vector<Tree<T>*>* force(int flag = 0)
	{
		if (!this->isForced()) {
			// call makeChilds function
			_vector_child_pointers_p = _rule_p->makeChilds(this, flag);

			// share game rule
			for (auto child_p : *_vector_child_pointers_p) {
				child_p->setRule(this->_rule_p);
				child_p->setParent_p(this);
			}
		}

		return _vector_child_pointers_p;
	}

	void forceRec(int max = 10, int flag = 0)
	{
		this->force(flag);

		if (max - 1 > 0) {
			for (auto child_p: *(this->force(flag)))
			{
				child_p->forceRec(max - 1, flag);
			}
		}
	}

	virtual void print(int depth = 0)
	{
		if (_action_p) {
			printDepth(depth);
			printf(">> ");
			_action_p->print();
			printf("\n");
		}

		_state_p->print(depth);
	}

	void printRec(int depthMax = 10, int depth = 0)
	{
		this->print(depth); 
		printf("\n");

		if (depth < depthMax && this->isForced()) {
			for (auto child_p: *(_vector_child_pointers_p)) {
				child_p->printRec(depthMax, depth + 1);
			}
		}
	}

	void setRule(BoardGame<T>* rule_p)
	{
		_rule_p = rule_p;
	}

private:
	void printDepth(int depth)
	{
		for (int i = 0; i < depth; i++)
			printf("_   ");
	}
};

} // end of namespace boardgame

#endif // boardgame__Tree_h_
