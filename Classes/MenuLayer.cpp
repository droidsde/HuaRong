#include "MenuLayer.h"
#include "cocostudio/CocoStudio.h"
#include "LevelScene.h"

using namespace cocostudio::timeline;
using namespace cocos2d::ui;
USING_NS_CC;
Scene* MenuLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuLayer::create();
	scene->addChild(layer);
	return scene;
}
MenuLayer::MenuLayer()
{

}

MenuLayer::~MenuLayer()
{

}

bool MenuLayer::init()
{
	Layer::init();

	Node * rootNode = CSLoader::createNode("MenuLayer.csb");
	this->addChild(rootNode);

	auto buttonStart = dynamic_cast<Button*>(rootNode->getChildByName("button_start"));
	buttonStart->addTouchEventListener(CC_CALLBACK_2(MenuLayer::buttonClickCallback, this));
	buttonStart->setTag(BUTTON_TAG_START);

	auto buttonClose = Button::create("CloseNormal.png","CloseSelected.png");
	buttonClose->setPosition(Vec2(20, 20));
	rootNode->addChild(buttonClose);
	buttonClose->addTouchEventListener(CC_CALLBACK_2(MenuLayer::buttonClickCallback, this));
	buttonClose->setTag(BUTTON_TAG_CLOSE);
	return true;
}

void MenuLayer::buttonClickCallback(Ref *pSender, Widget::TouchEventType type)
{
	log("button clicked");
	Node * pNode = dynamic_cast<Node*>(pSender);
	if (pNode->getTag() == BUTTON_TAG_START)
	{
		Director::getInstance()->replaceScene(LevelScene::createScene());
	}
	else if (pNode->getTag() == BUTTON_TAG_CLOSE)
	{
		Director::getInstance()->end();
	}
}