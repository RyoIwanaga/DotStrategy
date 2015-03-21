#include "TitleScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

enum Z
{
};

Scene* TitleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene->addChild(layer);

	return scene;
}

bool TitleScene::init()
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

	auto bg = LayerColor::create(Color4B::WHITE);
	this->addChild(bg);

	Sprite* spriteBg = Sprite::create("logo.jpg");
	spriteBg->setPosition(size / 2);
	this->addChild(spriteBg);

	// Event
    auto listener = EventListenerTouchOneByOne::create();
    // スワロータッチモードにするとonTouchBeganメソッドはタッチイベントは他では使われない。
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [](Touch* touch, Event* event)
   	{
    	Director::getInstance()->replaceScene(
    			TransitionFade::create(1.0f,
    					MainScene::createScene(),
						Color3B::WHITE));

    	return false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spriteBg);

	return true;
}
