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
		const Matrix4& getMatrix() const;
		const Node* getParent() const;
		bool getDirty() { return m_dirty; }
		bool getUseMatrix() { return m_useMatrix; }

		void setScale		(const Vector3& val);
		void setRotate		(const Quaternion& val);
		void setTranslate	(const Vector3& val);
		void setMatrix		(const Matrix4& val);
		void setParent		(Node* n);
		void setUseMatrix	(bool b) { m_useMatrix = b; }
		void setDirty		(bool b) { m_dirty = b; }

		std::vector<Node*> getChildren() const;
		void addChild(Node* n);

		void reset();
	private:
		union 
		{
			struct
			{
				Vector3		m_scale;
				Quaternion	m_rotate;
				Vector3		m_translate;
			};
			Matrix4		m_matrix;
		};

		Node* m_parent;
		std::vector<Node*> m_children;

		bool m_useMatrix;
		bool m_dirty;
	};
}