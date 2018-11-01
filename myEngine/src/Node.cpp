#include "Node.h"

namespace myEngine
{
	Node::Node()
	:m_scale(Vector3::ONE)
	,m_rotate()
	,m_translate(Vector3::ZERO)
	{

	}

	const Vector3& Node::getScale() const
	{
		return m_scale;
	}

	const Quaternion& Node::getRotate() const
	{
		return m_rotate;
	}

	const Vector3& Node::getTranslate() const
	{
		return m_translate;
	}

	void Node::setScale(const Vector3& val)
	{
		m_scale = val;
	}

	void Node::setRotate(const Quaternion& val)
	{
		m_rotate = val;
	}

	void Node::setTranslate(const Vector3& val)
	{
		m_translate = val;
	}
}
