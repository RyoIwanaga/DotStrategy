#ifndef _Strategy__BattleScene_h_
#define _Strategy__BattleScene_h_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "_debug.h"
#include "tactics/battle/Battle.h"

namespace tb = tactics::battle;

class BattleScene : public cocos2d::Layer
{
	CC_SYNTHESIZE(int, _boardWidth, BoardWidh);
	CC_SYNTHESIZE(int, _boardHeight, BoardHeight);

	CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite*>, _listSpritesUnit, ListSpritesUnit);
	CC_SYNTHESIZE(cocos2d::Vector<cocos2d::ui::LoadingBar*>, _listLoadingBarHp, ListLoadingBarHp);
	CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite*>, _listButtons, ListButtons);

	// game controller
	CC_SYNTHESIZE_PASS_BY_REF(tb::Battle, _game, Game);
	// root of game tree
	CC_SYNTHESIZE(boardgame::Tree<tb::State>*, _root, Root);

public:
	enum class EnumAi {
		HUMAN_VS_HUMAN,
		HUMAN_VS_AI,
		AI_VS_AI,
	};

	bool init(const tb::State& state, EnumAi ai);

	static BattleScene* create(const tb::State& state, EnumAi ai)
	{
		auto* pRet = new BattleScene();
		if (pRet && pRet->init(state, ai))
		{
			pRet->autorelease();
			return pRet; \
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	static cocos2d::Scene* createScene(const tb::State& state, EnumAi ai);

protected:
	BattleScene();
	virtual ~BattleScene();

	/*** Convert boardgame::Position to cocos2d::Vec2 ***/

	cocos2d::Vec2 getPosition(int x, int y);
	cocos2d::Vec2 getPosition(const boardgame::Point& pos);
	float getPositionX(int x);
	float getPositionY(int y);
	cocos2d::Vec2 addVec2Y(const cocos2d::Vec2 pos, float y);


	/*** Game ***/

	/** Synchronize current tree state and state of this scene
	 */
	void syncState(const tb::State& state);

	void hundleTree(boardgame::Tree<tb::State>* tree_p);
	void RESET_buttons();

	/*** Make button ***/

	void makeButton(boardgame::Tree<tb::State>* tree_p);
	void makeButtonWait(boardgame::Tree<tb::State>* tree_p);
	void makeButtonMove(boardgame::Tree<tb::State>* tree_p);
	void makeButtonAttackRange(boardgame::Tree<tb::State>* tree_p);
	void makeButtonAttackMelee(boardgame::Tree<tb::State>* tree_p);
	void makeButtonAttackMeleeExecute(
			const std::vector<boardgame::Tree<tb::State>*>& listTree_p);

	/*** Execute Action ***/

	void executeAction(boardgame::Tree<tb::State>* tree_p);


	/*** Animation ***/

	void mainAnimationMove(boardgame::Tree<tb::State>* tree_p,
			int indexUnit, const tb::Path& path, int indexPath = 1);
	void mainAnimationMelee(boardgame::Tree<tb::State>* tree_p,
			int indexUnit, int indexTarget, const tb::Path& path,
			boardgame::Point pTarget,
			const std::vector<tb::Attack>& attacks,
			int indexPath = 1);
	void mainAnimationRange(boardgame::Tree<tb::State>* tree_p,
			int indexUnit, int indexTarget,
			int damage, bool isDead,
			const boardgame::Point pFrom,
			const boardgame::Point pTarget);

	cocos2d::FiniteTimeAction* makeActionAttackMelee(
			const boardgame::Point& from,
			const boardgame::Point& to,
			int damage, bool isDead = false);

	void drawFloatDamage(int damage, const boardgame::Point& pos);
};

#endif // _Strategy__BattleScene_h_
