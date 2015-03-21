#ifndef _Strategy__TitleScene_h_
#define _Strategy__TitleScene_h_

#include "cocos2d.h"
// #include "ui/CocosGUI.h"

#include "_debug.h"
// #include "tactics/battle/Battle.h"

class TitleScene : public cocos2d::Layer
{
public:
	bool init() override;
	CREATE_FUNC(TitleScene);
	static cocos2d::Scene* createScene();

	TitleScene() {}
	virtual ~TitleScene() {}
};

#endif // _Strategy__TitleScene_h_
