#include "GameLayer.h"
#include "RoleSprite.h"
#include "MenuLayer.h"
#include "data/Role.h"
#include "data/Level.h"
#include "data/Profile.h"
USING_NS_CC ;
Scene* GameLayer::createScene()
{
	auto scene = Scene::create() ;
	auto layer = GameLayer::create() ;
	scene->addChild(layer);
	return scene ;
}
GameLayer::GameLayer()
	:m_pLevel(nullptr)
	,m_pSeleNode(nullptr)
	,m_touchBegin(Vec2(0,0))
	,m_touchMove(Vec2(0,0))
	,m_minDis(0)
{
	m_pLevel = Level::s_levelVec.at(Profile::getInstance()->getSeleLevelID()) ;
	CC_SAFE_RETAIN(m_pLevel) ;
	m_minDis = 20.0f ;
}

GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE_NULL(m_pLevel) ;
}
bool GameLayer::init()
{
	Size size = Director::getInstance()->getWinSize();
	
	auto bgSprite = Sprite::create("main_bg0.png") ;
	bgSprite->setPosition(size.width / 2,size.height / 2) ;
	addChild(bgSprite);

	initMap() ;

	auto lisener = EventListenerTouchOneByOne::create() ;
	lisener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this) ;
	lisener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved,this) ;
	lisener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lisener,this) ;

	return true ;
}

void GameLayer::initMap()
{
	Size size = Director::getInstance()->getWinSize();

	float x = 0 ;
	float y = 0 ;

	float paddingLeft   = 10.0f ;
	float paddingBottom = 53.0f ;

	for (unsigned i = 0 ; i < m_pLevel->getRoleID().size() ;i++)
	{
		Role * pRole = Role::s_roleVec.at(m_pLevel->getRoleID().at(i)) ;
		Sprite * pp = Sprite::create(pRole->getImageName()->getCString());
		int col = m_pLevel->getRolePos().at(i).x ;
		int row = m_pLevel->getRolePos().at(i).y ;
		x = paddingLeft + col * 1.0f * 75.0f ;
		y = paddingBottom + row * 1.0f * 75.0f ;
		RoleSprite * pRoleSprite = RoleSprite::create(pRole,row,col,Rect(x,y,pp->getContentSize().width,pp->getContentSize().height),this,this->getLocalZOrder()) ;
		m_pRoleSpriteVec.pushBack(pRoleSprite) ;
	}
	updateMapData() ;

	auto exit = Sprite::create("exit.png");
	x = paddingLeft + (m_pLevel->getExitCol() + 1) * 1.0f * 75.0f ;
	y = paddingBottom + m_pLevel->getExitRow() * 1.0f * 75.0f + 17.0f ;
	exit->setPosition(Vec2(x,y));
	addChild(exit, 20);//��θ�Щ
}

bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto loction = this->convertToNodeSpace(touch->getLocation()) ;
	
	for (unsigned int i = 0 ; i < m_pRoleSpriteVec.size() ; i++)
	{
		RoleSprite * pRoleSprite = m_pRoleSpriteVec.at(i) ;
		if (pRoleSprite->getBoundingBox().containsPoint(loction))
		{
			//�õ������λ�ú�ѡ�еĽ�ɫ
			m_touchBegin = loction ;
			m_pSeleNode = pRoleSprite ;
			break;
		}
	}

	return true ;
}
void GameLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	//��ɫ�ƶ������л��߽�ɫΪnull��ʱ����Ҫ����Ƿ��ƶ�
	if ((m_pSeleNode && m_pSeleNode->getNumberOfRunningActions() > 0) || !m_pSeleNode)
	{
		return ;
	}

	auto loction = this->convertToNodeSpace(touch->getLocation()) ;
	m_touchMove = loction ;
	float dis = m_touchMove.getDistance(m_touchBegin) ;//�����ƶ��ľ���
	auto var = m_touchMove - m_touchBegin ;
	auto moveByPos = Vec2(0,0) ;
	auto varColRow = Vec2(0,0) ;
	if (dis >= m_minDis)//�ƶ�
	{
		bool isMoveHor = abs(var.x) > abs(var.y) ;//�ж��Ǽ����ֱ������ƶ�����ˮƽ������ƶ�
		bool isMoveTop  ;//<�������ֱ�����ϣ��ж������ϻ�������
		bool isMoveRight ;//<�����ˮƽ�����ϣ��ж������һ�������
		if (isMoveHor)//ˮƽ�����ƶ�
		{
			isMoveRight = var.x > 0 ;
			if (var.x > 0)//right
			{
				moveByPos = Vec2(75.0f,0) ;
				varColRow = Vec2(1,0) ;
			}
			else//left
			{
				moveByPos = Vec2(-75.0f,0) ;
				varColRow = Vec2(-1,0) ;
			}
		}
		else//��ֱ�����ƶ�
		{
			isMoveTop = var.y > 0 ;
			if (var.y > 0)//top
			{
				moveByPos = Vec2(0,75.0f) ;
				varColRow = Vec2(0,1) ;
			}
			else//bottom
			{
				moveByPos = Vec2(0,-75.0f) ;
				varColRow = Vec2(0,-1) ;
			}
		}
		if (isCanMove(isMoveHor,isMoveHor ? isMoveRight : isMoveTop))
		{
			m_pSeleNode->setCol(m_pSeleNode->getCol() + (int)varColRow.x) ;
			m_pSeleNode->setRow(m_pSeleNode->getRow() + (int)varColRow.y) ;
			m_pSeleNode->runAction(
				Sequence::create(
				MoveBy::create(0.5f, moveByPos),
				CallFuncN::create(CC_CALLBACK_1(GameLayer::moveEndCallback, this)),nullptr
				)
				) ;
			updateMapData() ;//���µ�ͼ��״̬
		}
	}
}
void GameLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	//����
	m_touchBegin = Vec2(0,0) ;
	m_touchMove = Vec2(0,0) ;
	m_pSeleNode = nullptr ;
	//���ܲٵ�λ���Ƿ񵽴���ڣ������������ڣ���Ϸ����

}

bool GameLayer::isCanMove(bool isMoveHor,bool moveAdd)
{
	bool isMove = false;
	CCAssert(m_pSeleNode,"select node should not is null!!") ;
	int width = m_pSeleNode->getWidth() ;
	int height = m_pSeleNode->getHeight() ;
	int row = m_pSeleNode->getRow() ;
	int col = m_pSeleNode->getCol() ;
	if (isMoveHor)//ˮƽ����
	{
		if (moveAdd)
		{
			int colTem = col + width;
			int rowTem = row ;
 			int filledNum = 0 ;
 			for (int i = 0; i < height ; i++)
 			{
				rowTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
 			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getCol() == 3)
			{
				isMove = false ;
			}
		}
		else
		{
			int colTem = col - 1;
			int rowTem = row ;
			int filledNum = 0 ;
			for (int i = 0; i < height ; i++)
			{
				rowTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getCol() == 0)
			{
				isMove = false ;
			}
		}
	}
	else//��ֱ����
	{
		if (moveAdd)
		{
			int colTem = col ;
			int rowTem = row + height ;
			int filledNum = 0 ;
			for (int i = 0; i < width; i++)
			{
				colTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getHeight() == 2)
			{
				if (m_pSeleNode->getRow() == 3)
				{
					isMove = false ;
				}
			}
			else{
				if (m_pSeleNode->getRow() == 4)
				{
					isMove = false ;
				}
			}
		}
		else
		{
			int colTem = col ;
			int rowTem = row - 1 ;
			int filledNum = 0 ;
			for (int i = 0; i < width; i++)
			{
				colTem += i;
				if (m_mapState[rowTem][colTem] == 1)
				{
					filledNum++ ;
				}
			}
			isMove = (filledNum == 0) ? true : false;
			if (m_pSeleNode->getRow() == 0)//�����²��ʱ�򣬾Ͳ��������¶���
			{
				isMove = false ;
			}
		}
	}
	return isMove ;
}

void GameLayer::updateMapData()
{
	resetMapData() ;
	if (m_pRoleSpriteVec.size() == 10)//<��Ҫ10����ɫ1Boss��5������4��
	{
		for (unsigned int i = 0 ;i < m_pRoleSpriteVec.size() ;i++)
		{
			RoleSprite * pRoleSprite = m_pRoleSpriteVec.at(i) ;
			int width = pRoleSprite->getWidth() ;
			int heigit = pRoleSprite->getHeight() ;
			int row = pRoleSprite->getRow() ;
			int col = pRoleSprite->getCol() ;
			for (int i = 0 ; i < heigit ; i++)
			{
				for (int j = 0 ; j < width ; j++)
				{
					m_mapState[row + i][col + j] = 1 ;
				}
			}
		}
	}
	/*CCLOG("================Test==========================") ;
	for (int i = 0; i < 5 ; i++)
	{
	const char *ppp = "" ;
	for (int j = 0; j < 4; j++)
	{
	ppp = CCString::createWithFormat("%s%d,",ppp,m_mapState[i][j])->getCString() ;
	}
	CCLOG(ppp) ;
	}*/
}
void GameLayer::resetMapData()
{
	for (int row = 0 ;row < 5 ; row++)
	{
		for (int col = 0 ; col < 4 ; col++)
		{
			m_mapState[row][col] = 0; 
		}
	}
}

void GameLayer::moveEndCallback(Node* sender)
{
	for (const auto& roleSprite : m_pRoleSpriteVec)
	{
		if (kRoleTypeBoss == roleSprite->getRole()->getType())
		{
			if (roleSprite->getRow() == m_pLevel->getExitRow()
				&& roleSprite->getCol() == m_pLevel->getExitCol())
			{
				//��Ϸʤ��
				log("You Win!!");
				//�л����˵�����
				Director::getInstance()->replaceScene(MenuLayer::createScene());
			}
			return;
		}
	}
}