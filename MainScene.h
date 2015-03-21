#ifndef _Strategy__MainScene_h_
#define _Strategy__MainScene_h_

#include "cocos2d.h"
// #include "ui/CocosGUI.h"

#include "_debug.h"
// #include "tactics/battle/Battle.h"

class MainScene : public cocos2d::Layer
{
public:
	bool init() override;
	CREATE_FUNC(MainScene);
	static cocos2d::Scene* createScene();

protected:
	MainScene() {}
	virtual ~MainScene() {}
};

#endif // _Strategy__MainScene_h_
