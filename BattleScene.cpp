#include "BattleScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

const int FLOOR_WIDTH = 40;
const int FLOOR_HEIGHT = 40;

const int EDGE_WIDTH = 52;
const int EDGE_START_X = 32;
const int EDGE_START_Y = 28;

const int TARGET_ADD_Y = 30;

/*** Animation ***/

const float ANIMATION_MOVE_SPEED = 0.2;
const float CPU_DELAY = 0.5;

const float ARROW_DURATION = 1.0;
const float DAMAGE_DURATION = 1.2;
const float HEX_FLOAT_DULATION = 0.5;
const int DAMAGE_MOVE_Y = 20;

enum Z
{
	BG,
	FLOOR,
	FLOOR_SIGN,
	EDGE,
	UNIT,
	HP,
	CURSOR,
	ARROW,
	DAMAGE
};

BattleScene::BattleScene() :
		_boardWidth(9),
		_boardHeight(5),
		_root(nullptr)
{}

BattleScene::~BattleScene()
{
	delete _root;
}

Scene* BattleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = BattleScene::create();
	scene->addChild(layer);
	return scene;
}

bool BattleScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto size = Director::getInstance()->getWinSize();

	/*** Music ***/

	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/maounoshiro.mp3");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/maounoshiro.mp3");

	/*** Game ***/

	// Make state
	std::vector<tactics::battle::Unit*> units;

	// player 0
	units.push_back(new tactics::battle::UnitRanged(4, 90, 40, 4 ,5, 3, 7, 0,
				boardgame::Point(0, 0)));
	units.push_back(new tactics::battle::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 0,
				boardgame::Point(0, 1)));
	units.push_back(new tactics::battle::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 0,
				boardgame::Point(0, 2)));

	units.push_back(new tactics::battle::Unit(0, 130, 50, 4 ,5, 4, 8, 0,
				boardgame::Point(2, 2)));
	units.push_back(new tactics::battle::Unit(1, 130, 50, 4 ,5, 4, 8, 0,
				boardgame::Point(2, 3)));
	units.push_back(new tactics::battle::Unit(2, 130, 50, 4 ,5, 4, 8, 0,
				boardgame::Point(2, 4)));

	// player 2
	units.push_back(new tactics::battle::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 1,
				boardgame::Point(8, 2)));
	units.push_back(new tactics::battle::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 1,
				boardgame::Point(8, 3)));
	units.push_back(new tactics::battle::UnitRanged(4, 90, 40, 4 ,5, 3, 7, 1,
				boardgame::Point(8, 4)));

	units.push_back(new tactics::battle::Unit(2, 130, 50, 4 ,5, 4, 8, 1,
				boardgame::Point(5, 0)));
	units.push_back(new tactics::battle::Unit(1, 130, 50, 4 ,5, 4, 8, 1,
				boardgame::Point(5, 1)));
	units.push_back(new tactics::battle::Unit(0, 130, 50, 4 ,5, 4, 8, 1,
				boardgame::Point(5, 2)));

	auto state_p = new tactics::battle::State(2, _boardWidth, _boardHeight, units);

	// Make tree
	_root = new boardgame::Tree<tactics::battle::State>(
			state_p, &_game);

	_game.playerAddHuman();
	_game.playerAddAi(1);

	/*** Bg ***/

	Sprite* spriteBg = Sprite::create("bg.jpg");
	spriteBg->setPosition(size / 2);
	this->addChild(spriteBg, static_cast<int>(Z::BG));


	/*** Edge ***/

	for (int y = 0; y < _boardHeight; y++) {
		for (int x = 0; x < _boardWidth; x++) {

			// edge
			auto spriteEdge = Sprite::create("edge.png");
			spriteEdge->setPosition(this->getPosition(x, y));
			this->addChild(spriteEdge, static_cast<int>(Z::EDGE));

			// TODO place object (ex: rock)
		}
	}

	/*** Units ***/

	for (auto unit_p : units) {
		Sprite* spriteUnit_p;

		switch (unit_p->getId()) {
		case 0:
			spriteUnit_p = Sprite::create("melee.png");
			break;
		case 1:
			spriteUnit_p = Sprite::create("melee2.png");
			break;
		case 2:
			spriteUnit_p = Sprite::create("melee3.png");
			break;
		case 3:
			spriteUnit_p = Sprite::create("range.png");
			break;
		case 4:
			spriteUnit_p = Sprite::create("range2.png");
			break;
		}

		if (unit_p->getOwner() == 0) {
			spriteUnit_p->setFlipX(true);
		}

		spriteUnit_p->setPosition(this->getPosition(unit_p->getPos()));
		spriteUnit_p->setCascadeOpacityEnabled(true); // Apply child to opacity
		this->addChild(spriteUnit_p, static_cast<int>(Z::UNIT)); // XXX ちゃんと　x older を使用すること
		_listSpritesUnit.pushBack(spriteUnit_p); // PUSH

		/*** Hp ***/

		auto loadingBarHp_p = ui::LoadingBar::create("hp.png");
		loadingBarHp_p->setPercent(100);
		spriteUnit_p->addChild(loadingBarHp_p, static_cast<int>(Z::HP));
		loadingBarHp_p->setPosition(Vec2(EDGE_WIDTH / 2 - 6, 1)); // XXX
		_listLoadingBarHp.pushBack(loadingBarHp_p); // PUSH
	}

//	syncState(*state_p);
	hundleTree(_root);

	return true;
}

cocos2d::Vec2 BattleScene::getPosition(int x, int y)
{
	return Vec2(
			this->getPositionX(x),
			this->getPositionY(y));
}

cocos2d::Vec2 BattleScene::getPosition(const boardgame::Point& pos)
{
	return this->getPosition(pos.getX(), pos.getY());
}

float BattleScene::getPositionX(int x)
{
	return EDGE_START_X + x * EDGE_WIDTH;
}

float BattleScene::getPositionY(int y)
{
	return EDGE_START_Y + (_boardHeight - y - 1) * EDGE_WIDTH;
}

cocos2d::Vec2 BattleScene::addVec2Y(const cocos2d::Vec2 pos, float y)
{
	return Vec2(pos.x, pos.y + y);
}

void BattleScene::syncState(const tactics::battle::State& state)
{
	int i = 0;
	for (auto unit_p : state.getListUnitp()) {
		if (unit_p->isDead()) {
			_listSpritesUnit.at(i)->setOpacity(0);
		} else {
			_listSpritesUnit.at(i)->setOpacity(255); // XXX
			_listLoadingBarHp.at(i)->setPercent(unit_p->getHpParsent());
		}
		_listSpritesUnit.at(i)->setPosition(getPosition(
				unit_p->getPos()));

		i++;
	}
}

void BattleScene::hundleTree(boardgame::Tree<tactics::battle::State>* tree_p)
{
	log("%s", __func__);
	auto state_p = tree_p->getState_p();
	auto player = state_p->getPlayer();

	// Reset event
//	this->reset();
	syncState(*state_p);

	// game end ?
	tree_p->DELETE_askParentToDeleteBrother();
	tree_p->force();
	if (tree_p->isTerminal()) {
		log("%s game end", __func__);

//		std::vector<int> wins;
//		_game.collectWinner(&wins, *state_p);

		// TODO
	}
	// human
	else if (_game.listAi_p[player] == nullptr) {
		log("%s human", __func__);
		makeButton(tree_p);
	}
	// ai
	else {
		log("%s ai", __func__);
		auto index = _game.listAi_p[player]->choice(tree_p, player);
		auto heir_p = tree_p->force()->at(index);

		this->runAction(Sequence::create(
				DelayTime::create(CPU_DELAY),
				CallFunc::create([this, heir_p]()
				{
					this->executeAction(heir_p);
				}),
				NULL));
	}
}

void BattleScene::makeButton(boardgame::Tree<tb::State>* tree_p)
{
	log("%s", __func__);

	/*** Active unit ***/

	auto pActive = tree_p->getState_p()->getActiveUnit().getPos();

	/* draw blue floor */

	Sprite* sFloor = Sprite::create("floor_active.png");
	sFloor->setOpacity(180);
	sFloor->setPosition(getPosition(pActive));
	this->addChild(sFloor, static_cast<int>(Z::FLOOR_SIGN));
	_listButtons.pushBack(sFloor);	// PUSH

	/* cursol */

	Sprite* sActive = Sprite::create("target.png");
	sActive->setPosition(addVec2Y(
			getPosition(pActive), TARGET_ADD_Y));
	this->addChild(sActive, static_cast<int>(Z::CURSOR));
	_listButtons.pushBack(sActive);	// PUSH

	// animation: rotate forever
	sActive->runAction(RepeatForever::create(
			Sequence::create(
					RotateTo::create(1, Vec3(0, 180, 0)),
					RotateTo::create(1, Vec3(0, 360, 0)),
					NULL)));




	makeButtonMove(tree_p);
	makeButtonAttackMelee(tree_p);
	makeButtonAttackRange(tree_p);
}

void BattleScene::makeButtonAttackRange(boardgame::Tree<tb::State>* tree_p)
{
	log("%s", __func__);

	for (auto child_p : *(tree_p->force())) {
		auto cast = dynamic_cast<tb::ActionAttackRange*>(child_p->getAction_p());
		if (cast != nullptr) {

			/*** Target cursol ***/

			Sprite* sTarget = Sprite::create("from.png");
			sTarget->setPosition(addVec2Y(getPosition(cast->getPosTarget()),
					TARGET_ADD_Y));
			this->addChild(sTarget, static_cast<int>(Z::CURSOR));
			_listButtons.pushBack(sTarget);	// PUSH

			/*** Target floor ***/

			Sprite* sFloor = Sprite::create("floor_target.png");
			sFloor->setOpacity(180);
			sFloor->setPosition(getPosition(cast->getPosTarget()));
			this->addChild(sFloor, static_cast<int>(Z::FLOOR_SIGN));
			_listButtons.pushBack(sFloor);	// PUSH

			// animation
			sTarget->runAction(RepeatForever::create(Sequence::create(
					MoveBy::create(0.7, Vec2(0, -5)),
					MoveBy::create(0.1, Vec2(0, 5)),
					NULL)));

			// event
			auto listener_p = EventListenerTouchOneByOne::create();
			listener_p->setSwallowTouches(true);

			listener_p->onTouchBegan = [this, child_p] (Touch* touch, Event* event)
				{
					auto target = event->getCurrentTarget();

					if (target->getBoundingBox().containsPoint(
							touch->getLocation()))
					{
						this->RESET_buttons(); // RESET
						this->executeAction(child_p);
					}

					// 以降のイベントを処理しない
					return false;
				};

			// get target unit index
			int index;
			auto units = tree_p->getState_p()->getListUnitp();
			for (index = 0; index < units.size(); index++) {
				if (units.at(index)->getPos() == cast->getPosTarget())
					break;
			}

			this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
					listener_p, _listSpritesUnit.at(index));


		}
	}
}

void BattleScene::makeButtonMove(boardgame::Tree<tb::State>* tree_p)
{
	log("%s", __func__);

	for (auto child_p : *(tree_p->force())) {
		auto cast = dynamic_cast<tb::ActionMove*>(child_p->getAction_p());

		if (cast != nullptr) {
			Sprite* sFloor = Sprite::create("walk2.png");
			sFloor->setOpacity(180);
			sFloor->setPosition(this->getPosition(
					cast->getPath().back()));
			this->addChild(sFloor, static_cast<int>(Z::FLOOR_SIGN));
			_listButtons.pushBack(sFloor);

			// event
			auto listener_p = EventListenerTouchOneByOne::create();
			listener_p->setSwallowTouches(true);

			listener_p->onTouchBegan = [=] (Touch* touch, Event* event)
				{
				auto target = event->getCurrentTarget();

				if (target->getBoundingBox().containsPoint(
						touch->getLocation()))
				{
					this->executeAction(child_p);
				}

				// 以降のイベントを処理しない
				return false;
				};

			this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
					listener_p, sFloor);
		}
	}
}

void BattleScene::makeButtonAttackMelee(boardgame::Tree<tb::State>* tree_p)
{
	log("%s", __func__);

	// Collect target points
	std::set<boardgame::Point> pTargets;
	for (auto child_p : *(tree_p->force())) {
		auto cast = dynamic_cast<tb::ActionAttackMelee*>(child_p->getAction_p());

		if (cast != nullptr) {
			pTargets.insert(cast->getPosTarget());
		}
	}

	for (auto pTarget : pTargets) {
		std::vector<boardgame::Tree<tb::State>*> branches;

		// collect same target attack move
		for (auto child_p : *(tree_p->force())) {
			auto cast = dynamic_cast<tb::ActionAttackMelee*>(child_p->getAction_p());

			if (cast != nullptr && pTarget == cast->getPosTarget()) {
				branches.push_back(child_p);
			}
		}

		/*** Target cursol ***/

		Sprite* sTarget = Sprite::create("from.png");
		sTarget->setPosition(addVec2Y(getPosition(pTarget),
				TARGET_ADD_Y));
		this->addChild(sTarget, static_cast<int>(Z::CURSOR));
		_listButtons.pushBack(sTarget);	// PUSH

		/*** Target floor ***/

		Sprite* sFloor = Sprite::create("floor_target.png");
		sFloor->setOpacity(180);
		sFloor->setPosition(getPosition(pTarget));
		this->addChild(sFloor, static_cast<int>(Z::FLOOR_SIGN));
		_listButtons.pushBack(sFloor);	// PUSH

		// animation
		sTarget->runAction(RepeatForever::create(Sequence::create(
				MoveBy::create(0.7, Vec2(0, -5)),
				MoveBy::create(0.1, Vec2(0, 5)),
				NULL)));

		// event
		auto listener_p = EventListenerTouchOneByOne::create();
		listener_p->setSwallowTouches(true);

		listener_p->onTouchBegan = [this, branches] (Touch* touch, Event* event)
		{
			auto target = event->getCurrentTarget();

			if (target->getBoundingBox().containsPoint(
					touch->getLocation()))
			{
				this->RESET_buttons(); // RESET
				this->makeButtonAttackMeleeExecute(branches);
			}

			// 以降のイベントを処理しない
			return false;
		};

		// get target unit index
		int index;
		auto units = tree_p->getState_p()->getListUnitp();
		for (index = 0; index < units.size(); index++) {
			if (units.at(index)->getPos() == pTarget)
				break;
		}

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
				listener_p, _listSpritesUnit.at(index));
	}
}

void BattleScene::makeButtonAttackMeleeExecute(
		const std::vector<boardgame::Tree<tb::State>*>& listTree_p)
{
	for (auto tree_p : listTree_p) {
		auto cast = dynamic_cast<tb::ActionAttackMelee*>(tree_p->getAction_p());
		assert(cast != nullptr);

		auto from = cast->getPath().back();

		/*** Make button ***/

		Sprite* sTarget = Sprite::create("attack_x2.png");
		sTarget->setPosition(getPosition(from));
		this->addChild(sTarget, static_cast<int>(Z::FLOOR_SIGN));
		_listButtons.pushBack(sTarget);	// PUSH

		// event
		auto listener_p = EventListenerTouchOneByOne::create();
		listener_p->setSwallowTouches(true);

		listener_p->onTouchBegan = [this, tree_p] (Touch* touch, Event* event)
		{
			auto target = event->getCurrentTarget();

			if (target->getBoundingBox().containsPoint(
					touch->getLocation()))
			{
				this->RESET_buttons(); // RESET
				this->executeAction(tree_p);
			}

			// 以降のイベントを処理しない
			return false;
		};

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
				listener_p, sTarget);
	}
}


void BattleScene::executeAction(boardgame::Tree<tactics::battle::State>* tree_p)
{
	log("%s", __func__);
	this->RESET_buttons();

	auto action_p = tree_p->getAction_p();

	// ActionMove
	if (dynamic_cast<tb::ActionMove*>(action_p) != nullptr) {
		auto cast = dynamic_cast<tb::ActionMove*>(action_p);
		mainAnimationMove(tree_p, cast->getUnitIndex(),
				cast->getPath());
	}
	// ActionAttackMelee
	else if (dynamic_cast<tb::ActionAttackMelee*>(action_p) != nullptr) {
		auto cast = dynamic_cast<tb::ActionAttackMelee*>(action_p);
		mainAnimationMelee(tree_p,
				cast->getUnitIndex(),
				cast->getTargetIndex(),
				cast->getPath(),
				cast->getDamage(),
				cast->getIsTargetDead(),
				cast->getPosTarget());
	}
	// ActionAttackRange
	else if (dynamic_cast<tb::ActionAttackRange*>(action_p) != nullptr) {
		auto cast = dynamic_cast<tb::ActionAttackRange*>(action_p);
		mainAnimationRange(tree_p,
				cast->getUnitIndex(),
				cast->getTargetIndex(),
				cast->getDamage(),
				cast->getIsTargetDead(),
				cast->getPosFrom(),
				cast->getPosTarget());
	}
	else {
		log("%d::no action", __func__);
		this->hundleTree(tree_p);
	}
}

void BattleScene::mainAnimationMove(boardgame::Tree<tactics::battle::State>* tree_p,
			int indexUnit, const tactics::battle::Path& path,
			int indexPath) // = 1
{
	if (path.size() <= indexPath) {
		this->hundleTree(tree_p);
	}
	else {
		_listSpritesUnit.at(indexUnit)->runAction(Sequence::create(
					MoveTo::create(ANIMATION_MOVE_SPEED, getPosition(path.at(indexPath))),
					CallFunc::create([=]()
					{
						this->mainAnimationMove(tree_p, indexUnit, path, indexPath + 1);
					}),
					NULL));
	}
}

void BattleScene::mainAnimationMelee(boardgame::Tree<tb::State>* tree_p,
		int indexUnit, int indexTarget, const tb::Path& path,
		int damage, bool isDead, boardgame::Point pTarget,
		int indexPath) // = 1
{
	if (path.size() <= indexPath || path.size() == 1) {
		auto from = getPosition(path.back());
		auto target = getPosition(pTarget);

		_listSpritesUnit.at(indexUnit)->runAction(Sequence::create(
					MoveTo::create(ANIMATION_MOVE_SPEED, target),
					CallFunc::create([this, damage, pTarget]
					{
						this->drawFloatDamage(damage, pTarget);
					}),
					MoveTo::create(ANIMATION_MOVE_SPEED, from),
					CallFunc::create([this, tree_p]
					{
						this->hundleTree(tree_p);
					}),
					NULL));
	}
	// move
	else {
		_listSpritesUnit.at(indexUnit)->runAction(Sequence::create(
					MoveTo::create(ANIMATION_MOVE_SPEED, getPosition(path.at(indexPath))),
					CallFunc::create([=]()
					{
						this->mainAnimationMelee(tree_p, indexUnit, indexTarget,
								path, damage, isDead, pTarget, indexPath + 1);
					}),
					NULL));
	}
}

void BattleScene::mainAnimationRange(boardgame::Tree<tactics::battle::State>* tree_p,
			int indexUnit, int indexTarget,
			int damage, bool isDead,
			const boardgame::Point pFrom,
			const boardgame::Point pTarget)
{
	Sprite* arrow = Sprite::create("arrow.png");
	arrow->setFlippedX(true);
	arrow->setPosition(
			this->getPosition(pFrom));
	this->addChild(arrow);

	arrow->runAction(Sequence::create(
			MoveTo::create(0.6, this->getPosition(pTarget)),
			// REMOVE and draw damage
			CallFuncN::create([this, damage, pTarget, tree_p](Ref* ref)
			{
				auto node = dynamic_cast<Node*>(ref);
				node->removeFromParent();

				// draw damage
				this->drawFloatDamage(damage, pTarget);

				hundleTree(tree_p);
			}),
			NULL));
}

void BattleScene::drawFloatDamage(int damage, const boardgame::Point& pos)
{
	Label* labelDamage = Label::createWithSystemFont(
			StringUtils::toString(damage), "Arial", 28);
	labelDamage->setColor(Color3B::BLACK);
	labelDamage->setPosition(
			this->getPosition(pos));
	this->addChild(labelDamage, static_cast<int>(Z::DAMAGE));

	labelDamage->runAction(Sequence::create(
			Spawn::create(
					FadeOut::create(DAMAGE_DURATION),
					MoveTo::create(DAMAGE_DURATION, Vec2(
							this->getPositionX(pos.getX()),
							this->getPositionY(pos.getY()) + DAMAGE_MOVE_Y)),
					NULL),
			// DELETE
			CallFuncN::create([labelDamage](Ref* ref)
			{
				auto node = dynamic_cast<Node*>(ref);
				node->removeFromParent();
			}),
			NULL));
}

void BattleScene::RESET_buttons()
{
	for (Sprite* s : _listButtons)
	{
		s->removeFromParent();
	}

	_eventDispatcher->removeAllEventListeners(); // FIXME 本当はリスト上のものを削除
}
