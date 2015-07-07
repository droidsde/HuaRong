#include "Profile.h"

Profile * Profile::s_pProfile = nullptr;

Profile * Profile::getInstance()
{
	if (!s_pProfile)
	{
		s_pProfile = new Profile();
	}

	return s_pProfile;
}
bool  Profile::initStatic()
{
	s_pProfile = new Profile();
	return true;
}
void Profile::finalizeStatic()
{
	CC_SAFE_RELEASE_NULL(s_pProfile);
}
Profile::Profile()
	:m_seleLevelID(-1)
{
	m_seleLevelID = 0;//<默认选中的关切ID为0
}

Profile::~Profile()
{

}