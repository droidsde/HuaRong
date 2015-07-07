#include "LevelScene.h"
#include "cocostudio/CocoStudio.h"
#include "data/Profile.h"
#include "GameLayer.h"

using namespace cocostudio::timeline;
using namespace cocos2d::ui;
USING_NS_CC;
Scene * LevelScene::createScene()
{
	auto scene =  new LevelScene();
	scene->initSelf();
	scene->autorelease();
	return scene;
}
LevelScene::LevelScene()
{

}

LevelScene::~LevelScene()
{

}

bool LevelScene::initSelf()
{
	Node * rootNode = CSLoader::createNode("MainScene.csb");
	this->addChild(rootNode);
	auto node = dynamic_cast<Layout*>(rootNode->getChildByName("BG"));
	auto listView = dynamic_cast<ListView*>(node->getChildByName("ListView_3"));

	listView->addEventListener((ListView::ccListViewCallback)CC_CALLBACK_2(LevelScene::selectedItemEvent, this));
	//下面这个回调函数是不用的，只是为了测试
	listView->addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(LevelScene::selectedItemEventScrollView, this));
	return true;
}

void LevelScene::selectedItemEvent(Ref* pSender, ListView::EventType type)
{
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
	{
			ListView* listView = static_cast<ListView*>(pSender);
			CC_UNUSED_PARAM(listView);
			//CCLOG("select child start index = %ld", listView->getCurSelectedIndex());
			break;
	}
	case ListView::EventType::ON_SELECTED_ITEM_END:
	{
			ListView* listView = static_cast<ListView*>(pSender);
			CC_UNUSED_PARAM(listView);
			//CCLOG("select child end index = %ld", listView->getCurSelectedIndex());
			Profile::getInstance()->setSeleLevelID(listView->getCurSelectedIndex());
			log("LevelID:%d",Profile::getInstance()->getSeleLevelID());
			//切换界面
			Director::getInstance()->replaceScene(GameLayer::createScene());
			break;
	}
	default:
		break;
	}
}
void LevelScene::selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
	switch (type) {
	case ScrollView::EventType::SCROLL_TO_BOTTOM:
		//CCLOG("SCROLL_TO_BOTTOM");
		break;
	case ScrollView::EventType::SCROLL_TO_TOP:
		//CCLOG("SCROLL_TO_TOP");
		break;
	default:
		break;
	}
}