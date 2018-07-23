#include "Renderer.h"
#include "RenderObject.h"

namespace myEngine
{
	Renderer * Renderer::getInstance()
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
	bool Renderer::init()
	{
		return false;
	}
	
	void Renderer::render()
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (size_t i = 0; i < m_RenderObjects.size(); i++)
		{
			m_RenderObjects[i]->draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		// check for errors
		glCheckError();
	}
	void Renderer::setViewPort(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}
