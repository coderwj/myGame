#pragma once

#include <vector>

#include "Quaternion.h"
#include "Vector3.h"

namespace myEngine
{
	class Node
	{
	public:

		Node();
		~Node() { }
		
		const Vector3& getScale() const;
		const Quaternion& getRotate() const;
		const Vector3& getTranslate() const;
		const Node* getParent() const;

		void setScale		(const Vector3& val);
		void setRotate		(const Quaternion& val);
		void setTranslate	(const Vector3& val);
		void setParent		(Node* n);

		std::vector<Node*> getChildren() const;
		void addChild(Node* n);
	private:
		Vector3		m_scale;
		Quaternion	m_rotate;
		Vector3		m_translate;

		Node* m_parent;
		std::vector<Node*> m_children;
	};
}