#include "RenderObject.h"

#include "bgfx/bgfx.h"

namespace myEngine
{
	RenderObject::RenderObject()
	:m_material(nullptr)
	,m_vbh(BGFX_INVALID_HANDLE)
	,m_ibh(BGFX_INVALID_HANDLE)
	{
	
	}
	
	RenderObject::~RenderObject()
	{
		if (bgfx::isValid(m_vbh))
			bgfx::destroy(m_vbh);
		if (bgfx::isValid(m_ibh))
			bgfx::destroy(m_ibh);
	}
	
	void RenderObject::init(void * v_buf, int v_buf_size, void * i_buf, int i_buf_size)
	{
		bgfx::VertexDecl _dec;
		_dec
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
			.end();
		m_vbh = bgfx::createVertexBuffer(bgfx::copy(v_buf, v_size), _dec);
		m_ibh = bgfx::createIndexBuffer(bgfx::copy(ibuf, sizeof(uint16_t) * k));
	}

	void RenderObject::draw()
	{
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_EQUAL);
		bgfx::setIndexBuffer(m_ibh);
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::submit(0, m_program);
	}
}
