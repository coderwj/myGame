 #include "Animation.h"

#include <algorithm>

#include "MathUtil.h"
#include "Node.h"

namespace myEngine
{
	KeyChain::KeyChain()
	:m_type(KEY_CHAIN_TYPE_INVALID)
	,m_accType(KEY_CHAIN_ACCELERATE_LINEAR)
	,m_time(0.f)
	,m_loop(true)
	,m_target(nullptr)
	{

	}

	void KeyChain::tick(float time)
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

	const Node * KeyChain::getTarget() const
	{
		return m_target;
	}

	KEY_CHAIN_TARGET KeyChain::getTargetType() const
	{
		return m_targetType;
	}

	void KeyChain::setType(KEY_CHAIN_TYPE val)
	{
		m_type = val;
		m_values.reserve(val);
	}

	void KeyChain::setAccType(KEY_CHAIN_ACCELERATE val)
	{
		m_accType = val;
	}

	void KeyChain::setTarget(Node * val)
	{
		m_target = val;
	}

	void KeyChain::setTargetType(KEY_CHAIN_TARGET val)
	{
		m_targetType = val;
	}

	void KeyChain::addKeyFrame(const KeyFrame& frame)
	{
		m_keyFrames.push_back(frame);
	}

	void KeyChain::sortKeyFrames()
	{
		std::sort(m_keyFrames.begin(), m_keyFrames.end());
	}

	void KeyChain::_update()
	{
		if (m_keyFrames.size() == 0)
		{
			return;
		}
		m_values.clear();
		std::vector<KeyFrame>::iterator right = std::lower_bound(m_keyFrames.begin(), m_keyFrames.end(), KeyFrame(m_time));
		if (right == m_keyFrames.begin())
			m_values = m_keyFrames.begin()->values;
		else if (right == m_keyFrames.end())
		{
			m_values = (m_keyFrames.end() - 1)->values;
			if (m_loop)
			{
				//reStart
				m_time = 0.f;
			}
		}
		else
		{
			std::vector<KeyFrame>::iterator left = right - 1;

			float _k = (m_time - left->time) / (right->time - left->time);

			for (size_t i = 0; i < left->values.size(); i++)
			{
				float _result = lerpf(left->values[i], right->values[i], _k);
				m_values.push_back(_result);
			}
		}
		if (nullptr != m_target)
		{
			m_target->reset();
			switch (m_targetType)
			{
			case myEngine::KEY_CHAIN_TARGET_SCALE:
				m_target->setScale(Vector3(m_values[0], m_values[1], m_values[2]));
				break;
			case myEngine::KEY_CHAIN_TARGET_ROTATE:
				m_target->setRotate(Quaternion(m_values[0], m_values[1], m_values[2], m_values[3]));
				break;
			case myEngine::KEY_CHAIN_TARGET_TRANSLATE:
				m_target->setTranslate(Vector3(m_values[0], m_values[1], m_values[2]));
				break;
			default:
				break;
			}
			m_target->setDirty(true);
		}
	}

	void Animation::tick(int time)
	{
		for (KeyChain& c : m_keyChains)
		{
			c.tick(time * m_speed);
		}
	}

	void Animation::addKeyChain(const KeyChain& chain)
	{
		m_keyChains.push_back(chain);
	}

}
