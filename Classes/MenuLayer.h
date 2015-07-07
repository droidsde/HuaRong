#ifndef  _MENU_LAYER_H_
#define _MENU_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class MenuLayer : cocos2d::Layer
{
public:
	static const int BUTTON_TAG_START = 10;
	static const int BUTTON_TAG_CLOSE = 20;
	static cocos2d::Scene* createScene();
	CREATE_FUNC(MenuLayer);
public:
	MenuLayer();
	~MenuLayer();
	virtual bool init();

	void buttonClickCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	
private:

};
#endif