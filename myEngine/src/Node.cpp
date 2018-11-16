#include "Node.h"

namespace myEngine
{
	Node::Node()
	:m_scale(Vector3::ONE)
	,m_rotate()
	,m_translate(Vector3::ZERO)
	,m_parent(nullptr)
	,m_dirty(true)
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

	const Node* Node::getParent() const
	{
		return m_parent;
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

	void Node::setParent(Node* n)
	{
		m_parent = n;
	}

	std::vector<Node*> Node::getChildren() const
	{
		return m_children;
	}

	void Node::addChild(Node * n)
	{
		m_children.push_back(n);
	}

	void Node::reset()
	{
		m_scale = Vector3::ONE;
		m_rotate = Quaternion::IDENTITY;
		m_translate = Vector3::ZERO;
	}

}
