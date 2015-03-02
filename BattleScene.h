#ifndef _Strategy__BattleScene_h_
#define _Strategy__BattleScene_h_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "tactics/battle/Battle.h"

namespace tb = tactics::battle;

class BattleScene : public cocos2d::Layer
{
	CC_SYNTHESIZE(int, _boardWidth, BoardWidh);
	CC_SYNTHESIZE(int, _boardHeight, BoardHeight);

	CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite*>, _spritesUnit, SpritesUnit);
	CC_SYNTHESIZE(cocos2d::Vector<cocos2d::ui::LoadingBar*>, _listLoadingBarHp, ListLoadingBarHp);
	CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite*>, _listButtons, ListButtons);

	// game
	CC_SYNTHESIZE_PASS_BY_REF(tactics::battle::Battle, _game, Game);
	CC_SYNTHESIZE(boardgame::Tree<tactics::battle::State>*, _root, Root);

public:
	CREATE_FUNC(BattleScene);
	static cocos2d::Scene* createScene();

protected:
	BattleScene();
	virtual ~BattleScene();
	bool init() override;

private:
	/*** Convert boardgame::Position to cocos2d::Vec2 ***/

	cocos2d::Vec2 getPosition(int x, int y);
	cocos2d::Vec2 getPosition(const boardgame::Point& pos);
	float getPositionX(int x);
	float getPositionY(int y);
	cocos2d::Vec2 addVec2Y(const cocos2d::Vec2 pos, float y);

	/*** Game ***/

	void syncState(const tactics::battle::State& state);
	void hundleTree(boardgame::Tree<tb::State>* tree_p);

	/*** Make button ***/

	void makeButton(boardgame::Tree<tb::State>* tree_p);
	void makeButtonMove(boardgame::Tree<tb::State>* tree_p);
	void makeButtonAttackRange(boardgame::Tree<tb::State>* tree_p);
	void makeButtonAttackMelee(boardgame::Tree<tb::State>* tree_p);
	void makeButtonAttackMeleeExecute(const std::vector<boardgame::Tree<tb::State>*>& listTree_p);

	/*** Execute Action ***/

	void executeAction(boardgame::Tree<tactics::battle::State>* tree_p);

	/*** Animation ***/

	void mainAnimationMove(boardgame::Tree<tactics::battle::State>* tree_p,
			int indexUnit, const tactics::battle::Path& path, int indexPath = 1);
	void mainAnimationMelee(boardgame::Tree<tactics::battle::State>* tree_p,
			int indexUnit, int indexTarget, const tactics::battle::Path& path,
			int damage, bool isDead, boardgame::Point pTarget,
			int indexPath = 1);
	void mainAnimationRange(boardgame::Tree<tactics::battle::State>* tree_p,
			int indexUnit, int indexTarget,
			int damage, bool isDead,
			const boardgame::Point pFrom,
			const boardgame::Point pTarget);


	void drawFloatDamage(int damage, const boardgame::Point& pos);

	void RESET_buttons();
};

#endif // _Strategy__BattleScene_h_
