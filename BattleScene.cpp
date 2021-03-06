#include "BattleScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

const int FLOOR_WIDTH = 40;
const int FLOOR_HEIGHT = 40;

const int UI_HEIGHT = 52;
const int UI_LEFT_MARGIN = 6;

const int EDGE_WIDTH = 52;
const int EDGE_START_X = 32;
// const int EDGE_START_Y = 28;
const int EDGE_START_Y = UI_HEIGHT + 28;

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
	OBJ,
	FLOOR,
	FLOOR_SIGN,
	EDGE,
	UNIT,
	HP,
	UI_BG,
	UI,
	CURSOR,
	ARROW,
	DAMAGE,
};

BattleScene::BattleScene() :
		_boardWidth(0),
		_boardHeight(0),
		_root(nullptr)
{}

BattleScene::~BattleScene()
{
	delete _root;
}

Scene* BattleScene::createScene(const tb::State& state, EnumAi ai)
{
	auto scene = Scene::create();
	auto layer = BattleScene::create(state, ai);
	scene->addChild(layer);
	return scene;
}

bool BattleScene::init(const tb::State& state, EnumAi ai)
{
	if (!Layer::init()) {
		return false;
	}

	auto size = Director::getInstance()->getWinSize();

	/*** Music and SE***/

	// Music
	if (DEBUG::MUSIC) {
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.9);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bm/Future Gladiator.mp3");
	}

	// SE setting
	if (DEBUG::SE) {
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.9);
	}

	/*** Game ***/

	_boardWidth = state.getWidth();
	_boardHeight = state.getHeight();

	// copy
	auto state_p = new tb::State(state);

	// Make tree
	_root = new boardgame::Tree<tb::State>(
			state_p, &_game);

	/*** Set ai ***/

	switch (ai) {
	case EnumAi::HUMAN_VS_AI:
		_game.playerAddHuman();
		_game.playerAddAi(2);
		break;
	case EnumAi::AI_VS_AI:
		_game.playerAddAi(2);
		_game.playerAddAi(2);
		break;
	case EnumAi::HUMAN_VS_HUMAN:
		_game.playerAddHuman();
		_game.playerAddHuman();
		break;
	}


	/*** Bg ***/

	Sprite* spriteBg = Sprite::create("bg.jpg");
	spriteBg->setPosition(Vec2(
			size.width / 2,
			size.height / 2 + UI_HEIGHT));
	this->addChild(spriteBg, static_cast<int>(Z::BG));

	// ui
	Sprite* spriteUiBg = Sprite::create("battle_ui_bg.png");
	spriteUiBg->setPosition(Vec2(
			size.width / 2,
			UI_HEIGHT / 2));
	this->addChild(spriteUiBg, static_cast<int>(Z::UI_BG));

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

	for (auto unit_p : state_p->getListUnitp()) {
		Sprite* spriteUnit_p;

		switch (unit_p->getId()) {
		case 0:
			spriteUnit_p = Sprite::create("img/unit/frog.png");
//			spriteUnit_p = Sprite::create("unit/dog.png");
			break;
		case 3:
			spriteUnit_p = Sprite::create("img/unit/eye.png");
			break;
		case 4:
			spriteUnit_p = Sprite::create("img/unit/skelton.png");
			break;
		}

		if (unit_p->getOwner() == 0) {
			spriteUnit_p->setFlipX(true);
		}

		spriteUnit_p->setPosition(this->getPosition(unit_p->getPos()));
		spriteUnit_p->setCascadeOpacityEnabled(true); // Apply child to opacity
		this->addChild(spriteUnit_p, static_cast<int>(Z::UNIT));
		_listSpritesUnit.pushBack(spriteUnit_p); // PUSH

		/*** Hp ***/

		auto loadingBarHp_p = ui::LoadingBar::create("hp.png");
		loadingBarHp_p->setPercent(100);
		spriteUnit_p->addChild(loadingBarHp_p, static_cast<int>(Z::HP));
		loadingBarHp_p->setPosition(Vec2(EDGE_WIDTH / 2 - 6, 1)); // XXX
		_listLoadingBarHp.pushBack(loadingBarHp_p); // PUSH
	}

	// place object
	for (tb::Floor rock : state.getListFloor()) {
		Sprite* spriteRock = Sprite::create("obj_rock.png");
		spriteRock->setPosition(getPosition(rock.getPos()));
		this->addChild(spriteRock, static_cast<int>(Z::OBJ));
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

void BattleScene::syncState(const tb::State& state)
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

void BattleScene::hundleTree(boardgame::Tree<tb::State>* tree_p)
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
		if (DEBUG::MUSIC) {
			SimpleAudioEngine::sharedEngine()->
					playBackgroundMusic("bm/Take a Chance.mp3");
		}

//		std::vector<int> wins;
//		_game.collectWinner(&wins, *state_p);

		// TODO
	}
	// human
	else if (_game.getListAi_p().at(player) == nullptr) {
		makeButton(tree_p);
	}
	// ai
	else {
		log("%s ai", __func__);
		auto index = _game.getListAi_p().at(player)->choice(tree_p, player);
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

	Sprite* sActive = Sprite::create("active.png");
//	Sprite* sActive = Sprite::create("active.png");
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

	makeButtonWait(tree_p);
	makeButtonMove(tree_p);
	makeButtonAttackMelee(tree_p);
	makeButtonAttackRange(tree_p);
}

void BattleScene::makeButtonWait(boardgame::Tree<tb::State>* tree_p)
{
	log("%s", __func__);

	for (auto child_p : *(tree_p->force())) {
		auto cast = dynamic_cast<tb::ActionWait*>(child_p->getAction_p());

		if (cast != nullptr) {
			Sprite* sButtonWait = Sprite::create("battle_pass.png");
			sButtonWait->setPosition(UI_HEIGHT / 2 + UI_LEFT_MARGIN, UI_HEIGHT / 2);
			this->addChild(sButtonWait, static_cast<int>(Z::UI));
			_listButtons.pushBack(sButtonWait);

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

				return false;
			};

			this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
					listener_p, sButtonWait);
		}
	}
}

void BattleScene::makeButtonAttackRange(boardgame::Tree<tb::State>* tree_p)
{
	log("%s", __func__);

	for (auto child_p : *(tree_p->force())) {
		auto cast = dynamic_cast<tb::ActionAttackRange*>(child_p->getAction_p());
		if (cast != nullptr) {

			/*** Target cursol ***/

			Sprite* sTarget = Sprite::create("target.png");
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

					// 莉･髯阪�ｮ繧､繝吶Φ繝医ｒ蜃ｦ逅�縺励↑縺�
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

		Sprite* sTarget = Sprite::create("target.png");
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

		Sprite* sTarget = Sprite::create("attack.png");
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

			return false;
		};

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
				listener_p, sTarget);
	}
}


void BattleScene::executeAction(boardgame::Tree<tb::State>* tree_p)
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
	// ActionWait
	else if (dynamic_cast<tb::ActionWait*>(action_p) != nullptr) {
		auto cast = dynamic_cast<tb::ActionWait*>(action_p);

		// todo
		hundleTree(tree_p);
	}
	// ActionAttackMelee
	else if (dynamic_cast<tb::ActionAttackMelee*>(action_p) != nullptr) {
		auto cast = dynamic_cast<tb::ActionAttackMelee*>(action_p);
		mainAnimationMelee(tree_p,
				cast->getUnitIndex(),
				cast->getTargetIndex(),
				cast->getPath(),
				cast->getPosTarget(),
				cast->getAttacks());
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

void BattleScene::mainAnimationMove(boardgame::Tree<tb::State>* tree_p,
			int indexUnit, const tb::Path& path,
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
		boardgame::Point pTarget,
		const std::vector<tb::Attack>& attacks,
		int indexPath) // = 1
{
	if (path.size() <= indexPath || path.size() == 1) {
		boardgame::Point pUnit = path.back();
		Vec2 from = getPosition(pUnit);
		Vec2 target = getPosition(pTarget);

		// retaliation
		if (attacks[1].isHaveDamage()) {
			Sprite* sTarget = _listSpritesUnit.at(indexTarget);

			_listSpritesUnit.at(indexUnit)->runAction(Sequence::create(
					makeActionAttackMelee(pUnit, pTarget,
							attacks[0].getDamage(),
							attacks[0].getIsDead()),
					CallFunc::create([this, tree_p, sTarget, pTarget, pUnit, attacks]
					{
						sTarget->runAction(Sequence::create(
								makeActionAttackMelee(pTarget, pUnit,
										attacks[1].getDamage(),
										attacks[1].getIsDead()),
								CallFunc::create([this, tree_p]
								{
									this->hundleTree(tree_p);
								}),
								NULL));
					}),
					NULL));
		}
		// no retaliation
		else {
			_listSpritesUnit.at(indexUnit)->runAction(Sequence::create(
					makeActionAttackMelee(pUnit, pTarget,
							attacks[0].getDamage(),
							attacks[0].getIsDead()),
					CallFunc::create([this, tree_p]
					{
						this->hundleTree(tree_p);
					}),
					NULL));
		}
	}
	// move
	else {
		_listSpritesUnit.at(indexUnit)->runAction(Sequence::create(
					MoveTo::create(ANIMATION_MOVE_SPEED, getPosition(path.at(indexPath))),
					CallFunc::create([=]()
					{
						this->mainAnimationMelee(tree_p, indexUnit, indexTarget,
								path, pTarget, attacks, indexPath + 1);
					}),
					NULL));
	}
}

void BattleScene::mainAnimationRange(boardgame::Tree<tb::State>* tree_p,
			int indexUnit, int indexTarget,
			int damage, bool isDead,
			const boardgame::Point pFrom,
			const boardgame::Point pTarget)
{
	Sprite* arrow = Sprite::create("arrow.png");
//	arrow->setFlippedX(true);
	arrow->setRotation(
			90 - reu::diagram::degree(
					pFrom.getX(),
					pFrom.getY(),
					pTarget.getX(),
					pTarget.getY()));
	arrow->setPosition(
			this->getPosition(pFrom));
	this->addChild(arrow);

	arrow->runAction(Sequence::create(
			CallFunc::create([]()
			{
				if(DEBUG::SE)
					SimpleAudioEngine::sharedEngine()->playEffect("se/attack1.mp3");
			}),
			MoveTo::create(0.6, this->getPosition(pTarget)),
			// REMOVE and draw damage
			CallFuncN::create([this, damage, pTarget, tree_p, isDead](Ref* ref)
			{
				auto node = dynamic_cast<Node*>(ref);
				node->removeFromParent();

				if(DEBUG::SE)
					SimpleAudioEngine::sharedEngine()->playEffect("se/damage1.mp3");

				if (isDead) {
					if(DEBUG::SE)
						SimpleAudioEngine::sharedEngine()->playEffect("se/devil_groaning1.mp3");
				}

				// draw damage
				this->drawFloatDamage(damage, pTarget);

				hundleTree(tree_p);
			}),
			NULL));
}

FiniteTimeAction* BattleScene::makeActionAttackMelee(
		const boardgame::Point& from,
		const boardgame::Point& to,
		int damage,
		bool isDead) // = false
{
	return Sequence::create(
			MoveTo::create(ANIMATION_MOVE_SPEED, getPosition(to)),
			CallFunc::create([this, from, to, damage, isDead]
			{
				if(DEBUG::SE)
					SimpleAudioEngine::sharedEngine()->
					playEffect("se/straight_punch.mp3");

				if (isDead) {
					if(DEBUG::SE)
						SimpleAudioEngine::sharedEngine()->
						playEffect("se/devil_groaning1.mp3");
				}
				this->drawFloatDamage(damage, to);
			}),
			MoveTo::create(ANIMATION_MOVE_SPEED, getPosition(from)),
			NULL);
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

	_eventDispatcher->removeAllEventListeners(); // FIXME
}
