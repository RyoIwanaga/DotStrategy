#include "MainScene.h"
#include "BattleScene.h"
#include "SimpleAudioEngine.h"

#include "tactics/battle/Battle.h"

USING_NS_CC;
using namespace CocosDenshion;

const float DURATION_GO_BATTLE = 1.5f;

enum Z
{
};

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);

	return scene;
}

bool MainScene::init()
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

	/*** Bg ***/

	Sprite* bg = Sprite::create("main_bg.jpg");
	bg->setPosition(size / 2);
	this->addChild(bg);

	/*** Ui ***/

	Vector<MenuItem*> menuItems;
	MenuItemImage* button;
	Label* label;

	// Make state
	std::vector<tb::Unit*> units;

	// player 0
	units.push_back(new tb::UnitRanged(4, 90, 40, 4 ,5, 3, 7, 0,
				boardgame::Point(0, 0)));
	units.push_back(new tb::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 0,
				boardgame::Point(0, 1)));
	units.push_back(new tb::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 0,
				boardgame::Point(0, 2)));

	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 0,
				boardgame::Point(2, 1)));
	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 0,
				boardgame::Point(2, 2)));
	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 0,
				boardgame::Point(2, 3)));
	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 0,
				boardgame::Point(2, 4)));

	// player 2
	units.push_back(new tb::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 1,
				boardgame::Point(8, 2)));
	units.push_back(new tb::UnitRanged(3, 90, 40, 4 ,5, 3, 7, 1,
				boardgame::Point(8, 3)));
	units.push_back(new tb::UnitRanged(4, 90, 40, 4 ,5, 3, 7, 1,
				boardgame::Point(8, 4)));

	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 1,
				boardgame::Point(6, 0)));
	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 1,
				boardgame::Point(6, 1)));
	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 1,
				boardgame::Point(6, 2)));
	units.push_back(new tb::Unit(0, 130, 50, 4 ,5, 4, 8, 1,
				boardgame::Point(6, 3)));

	// make rocks
	std::vector<tb::Floor> rocks;
	tb::Floor::collectRocks(&rocks, 9, 5, 4, 3);

	tb::State state(2, 9, 5, units, rocks);

	// Tutorial TODO
	button = MenuItemImage::create("stage.png", "stage_press.png", [state](Ref* ref)
	{
	//	Director::getInstance()->replaceScene(
	//			TransitionFadeTR::create(DURATION_GO_BATTLE,
	//					BattleScene::createScene(state, BattleScene::Ai::HUMAN_VS_AI)));
	});
	label = Label::create("チュートリアル(Coming Soon)", "arial", 24.0);
	label->setColor(Color3B::WHITE);
	label->enableOutline(Color4B::BLACK);
	button->addChild(label);
	label->setPosition(button->getContentSize() / 2);
	label->setColor(Color3B::BLACK);
    menuItems.pushBack(button);

	// play vs ai
	button = MenuItemImage::create("stage.png", "stage_press.png", [state](Ref* ref)
	{
		Director::getInstance()->replaceScene(
				TransitionFadeTR::create(DURATION_GO_BATTLE,
						BattleScene::createScene(state, BattleScene::EnumAi::HUMAN_VS_AI)));
	});
	label = Label::create("vs AI", "arial", 24.0);
	label->setColor(Color3B::BLACK);
	button->addChild(label);
	label->setPosition(button->getContentSize() / 2);
	label->setColor(Color3B::BLACK);
    menuItems.pushBack(button);

	// play vs human
	button = MenuItemImage::create("stage.png", "stage_press.png", [state](Ref* ref)
	{
		Director::getInstance()->replaceScene(
				TransitionFadeTR::create(DURATION_GO_BATTLE,
						BattleScene::createScene(state, BattleScene::EnumAi::HUMAN_VS_HUMAN)));
	});
	label = Label::create("vs Human", "arial", 24.0);
	label->setColor(Color3B::BLACK);
	button->addChild(label);
	label->setPosition(button->getContentSize() / 2);
	label->setColor(Color3B::BLACK);
    menuItems.pushBack(button);


	Menu* menu = Menu::createWithArray(menuItems);
	this->addChild(menu);
	menu->alignItemsVertically();

	return true;
}
