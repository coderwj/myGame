#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include <vector>

typedef struct {
  GLuint vb;    // vertex buffer
  int numTriangles;
} RenderObject;

class RenderManager
{
private:
	static RenderManager * rm;
	std::vector<RenderObject> m_RenderObjects;
public:
	static RenderManager * getInstance()
	{
		if(rm == NULL)
			rm = new RenderManager();
		return rm;
	}
	void destroyInstance(){
		if(rm)
		{
			delete(rm);
			rm = NULL;
		}
	}

	bool init();

	void render();
};


#endif //__RENDERMANAGER_H__