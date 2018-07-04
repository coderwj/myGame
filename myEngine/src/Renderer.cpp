#include "Renderer.h"
#include "RenderObject.h"

namespace myEngine
{
	bool Renderer::init()
	{
		return false;
	}
	
	void Renderer::render()
	{
		for (size_t i = 0; i < m_RenderObjects.size(); i++)
		{
			m_RenderObjects[i]->draw();
		}
	}
}
