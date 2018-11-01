 #include "Animation.h"

#include <algorithm>

#include "MathUtil.h"

namespace myEngine
{
	KeyChain::KeyChain()
	:m_type(KEY_CHAIN_TYPE_INVALID)
	,m_accType(KEY_CHAIN_ACCELERATE_LINEAR)
	,m_time(0.f)
	,m_loop(false)
	{

	}

	void KeyChain::tick(int time)
	{
		m_time += time / 1000.f;
		_update();
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

	void KeyChain::_update()
	{
		/*if (m_keyFrames.size() == 0)
		{
			return;
		}
		std::vector<float> _value;
		_value.reserve(m_keyFrames.begin()->values.size());
		std::vector<KeyFrame>::iterator right = std::lower_bound(m_keyFrames.begin(), m_keyFrames.end(), KeyFrame(m_time));
		if (right == m_keyFrames.begin())
			_value = m_keyFrames.begin()->values;
		else
		{
			std::vector<KeyFrame>::iterator left = right - 1;

			float _k = (m_time - left->time) / (right->time - left->time);

			for (size_t i = 0; i < left->values.size(); i++)
			{
				float _result = lerpf(left->values[i], right->values[i], _k);
				_value.push_back(_result);
			}
		}*/
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
