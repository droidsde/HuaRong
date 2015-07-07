#ifndef _MY_SCENE_H_
#define _MY_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LevelScene : public cocos2d::Scene
{
public:
	static Scene * createScene();
public:
	LevelScene();
	~LevelScene();
	virtual bool initSelf();

	void selectedItemEvent(cocos2d::Ref* pSender, cocos2d::ui::ListView::EventType type);
	void selectedItemEventScrollView(cocos2d::Ref* pSender, cocos2d::ui::ScrollView::EventType type);
private:

};

#endif