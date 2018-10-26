 #include "Animation.h"
namespace myEngine
{
	KEY_FRAME_TYPE AnimChain::getType() const
	{
		return m_type;
	}
	void AnimChain::setType(myEngine::KEY_FRAME_TYPE val)
	{
		m_type = val;
	}

}
