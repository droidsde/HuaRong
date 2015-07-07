#ifndef _PROFILE_H_
#define _PROFILE_H_

#include "cocos2d.h"

class Profile : public cocos2d::Ref
{
public:
	static Profile * s_pProfile;
	static Profile * getInstance();
	static bool  initStatic();
	static void finalizeStatic();
public:
	Profile();
	~Profile();
	//@biref 选中的关卡的ID
	CC_SYNTHESIZE(int ,m_seleLevelID,SeleLevelID);
private:

};

#endif