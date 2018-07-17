#pragma once

#include <vector>


namespace myEngine
{
	class RenderObject;
	
	class Renderer
	{
	private:
		static Renderer * r;
		std::vector<RenderObject*> m_RenderObjects;
	public:
		static Renderer * getInstance()
		{
			if(r == NULL)
				r = new Renderer();
			return r;
		}
		static void destroyInstance(){
			if(r)
			{
				delete(r);
				r = NULL;
			}
		}
	
		bool init();
	
		void render();
	};
}
