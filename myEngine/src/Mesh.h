#pragma once

#include <string>
#include <vector>

#include "bgfx/bgfx.h"

namespace myEngine
{
	class Model;
	class Node;
	class  Material;

	class Primptive
	{

	private:
		Model* m_model;
		Node* m_node;
		Material*	m_material;
		bgfx::VertexBufferHandle	m_vbh;
		bgfx::IndexBufferHandle		m_ibh;
	};
	
	class Mesh {
	public:
		Mesh(){}
		~Mesh(){}
	private:
		Node* m_attachNode;
		std::vector<Primptive> m_primptives;
	};
}