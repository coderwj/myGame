#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__


#include <string>
#include <vector>
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
	std::vector<Character *> m_characters;

	struct lua_State * m_state;

	GameScene():
	m_mainCharacter(NULL),
	m_camera(NULL),
	m_state(NULL)
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