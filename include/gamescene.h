#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "MyEngineCore.h"
#include <string>
#include <vector>
using namespace std;

class Camera;
class Character;
class Model;
class Shader;

struct FogPara{
	Vector3 color;
	float	start;
	float	end;
	float	intensity;
};

class GameScene{
private:
	static GameScene * gs;

	Character * m_mainCharacter;
	Camera * m_camera;

	Model * m_model;
	Shader * m_shader;

	float m_scale;
	float m_theta;
	Vector3 m_rotateVec;

	std::vector<Character *> m_characters;

	struct lua_State * m_state;

	float m_nowTime;

	struct FogPara m_fogpara;

	GameScene():
		m_mainCharacter(NULL),
		m_camera(NULL),
		m_scale(1.0f),
		m_theta(0.0f),
        m_state(NULL),
		m_nowTime(0)
	{
		m_rotateVec = Vector3(0.0f, 1.0f, 0.0f);
		m_fogpara.color = Vector3(1.0f, 1.0f, 1.0f);
		m_fogpara.start = 10.f;
		m_fogpara.end = 100.f;
		m_fogpara.intensity = 1.f;
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

	void changeScene(string scenename);
};

#endif //__GAMESCENE_H__
