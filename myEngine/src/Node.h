#pragma once

#include <vector>

#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4.h"

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
		Node* getParent() const;
		bool getDirty() { return m_dirty; }

		void setScale		(const Vector3& val);
		void setRotate		(const Quaternion& val);
		void setTranslate	(const Vector3& val);
		void setParent		(Node* n);
		void setDirty		(bool b) { m_dirty = b; }

		std::vector<Node*> getChildren() const;
		void addChild(Node* n);

		void reset();

		const Matrix4& generateGlobalMatrix();
	private:
		Vector3		m_scale;
		Quaternion	m_rotate;
		Vector3		m_translate;

		Matrix4		m_globalMatrix;

		Node* m_parent;
		std::vector<Node*> m_children;

		bool m_dirty;
	};
}