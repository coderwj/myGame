#pragma once
#include "bgfx/bgfx.h"

namespace myEngine
{
	class Material;

	class RenderObject
	{
	private:
		Material*	m_material;
		bgfx::VertexBufferHandle	m_vbh;
		bgfx::IndexBufferHandle		m_ibh;
	public:
		RenderObject();
		~RenderObject();
	
		void init(void * v_buf, int v_buf_size, void * i_buf, int i_buf_size);
		void draw();
	};
}
