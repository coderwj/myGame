#pragma once

#include <vector>


namespace myEngine
{
	class RenderObject;
	
	class Renderer
	{
	public:
		static Renderer * getInstance();

		static void destroyInstance();
	
		bool init();
	
		void render();

		void setViewPort(int width, int height);

	private:
		static Renderer * r;
		std::vector<RenderObject*> m_RenderObjects;
	};
}
