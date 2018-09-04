#include "Renderer.h"

#include "RenderObject.h"
#include "bgfx/bgfx.h"

namespace myEngine
{
	Renderer* Renderer::r = NULL;

	Renderer* Renderer::getInstance()
	{
		if (r == NULL)
			r = new Renderer();
		return r;
	}
	void Renderer::destroyInstance() {
		if (r)
		{
			delete(r);
			r = NULL;
		}
	}
	Renderer::Renderer()
	:m_viewport_width(1280)
	,m_viewport_height(720)
	{

	}
	Renderer::~Renderer()
	{
	}
	bool Renderer::init()
	{
		bgfx::Init init;
		init.type = bgfx::RendererType::OpenGLES;
		init.vendorId = BGFX_PCI_ID_NONE; //auto select
		init.resolution.width = m_viewport_width;
		init.resolution.height = m_viewport_height;
		init.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(init);

		// Enable debug text.
		bgfx::setDebug(BGFX_DEBUG_NONE);

		clear();

		return false;
	}
	
	void Renderer::render()
	{
		for (size_t i = 0; i < m_RenderObjects.size(); i++)
		{
			m_RenderObjects[i]->draw();
		}
	}
	void Renderer::setViewPort(int width, int height)
	{
		m_viewport_width = width;
		m_viewport_height = height;
		bgfx::setViewRect(0, 0, 0, width, height);
	}
	void Renderer::clearDepth() const
{
		bgfx::setViewClear(0, BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
	}
	void Renderer::clearColor() const
{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x303030ff, 1.0f, 0);
	}
	void Renderer::clear() const
{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
	}
}
