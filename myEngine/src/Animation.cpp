 #include "Animation.h"
namespace myEngine
{
	void KeyChain::tick(int time)
	{

	}
	KEY_CHAIN_TYPE KeyChain::getType() const
	{
		return m_type;
	}

	KEY_CHAIN_ACCELERATE KeyChain::getAccType() const
	{
		return m_accType;
	}

	void KeyChain::setType(myEngine::KEY_CHAIN_TYPE val)
	{
		m_type = val;
	}

	void KeyChain::setAccType(myEngine::KEY_CHAIN_ACCELERATE val)
	{
		m_accType = val;
	}

	void KeyChain::addKeyFrame(const KeyFrame& frame)
	{
		m_keyFrames.push_back(frame);
	}

	void Animation::tick(int time)
	{
		for (KeyChain& c : m_keyChains)
		{
			c.tick(time);
		}
	}

	void Animation::addKeyChain(const KeyChain& chain)
	{
		m_keyChains.push_back(chain);
	}

}
