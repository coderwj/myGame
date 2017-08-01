#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include <string>
using namespace std;

class Camera;
class Character;
class Model;
class Shader;

class GameScene{
private:
	static GameScene * gs;

	Character * m_mainCharacter;
	Camera * m_camera;

	Model * m_model;
	Shader * m_shader;

	GameScene():
	m_mainCharacter(NULL),
	m_camera(NULL)
	{
	}
	~GameScene(){}
	void onDestroy();
public:
	static GameScene * getInstance(){
		if(gs == NULL)
			gs = new GameScene();
		return gs;
	}

	void destroyInstance(){
		if(gs)
		{
			gs->onDestroy();
			delete(gs);
			gs = NULL;
		}
	}

	bool init();

	void render();

	void renderScene();

	void tick(float delta);

	Camera * getCamera()
	{
		return m_camera;
	}

	void loadScene(string scenename);
};

#endif //__GAMESCENE_H__