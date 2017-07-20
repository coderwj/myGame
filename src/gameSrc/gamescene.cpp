
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

#include "MyEngineCore.h"
#include "GameScene.h"

#include "Config.h"
#include "Shader.h"


#include <thread>
#include <sstream>

using namespace std;

#ifdef WIN32
    const string Config::engine_res_path = "../../../../../myEngine/res/";
	const string Config::game_res_path = "../../../../../res/";
	const string Config::model_path = Config::game_res_path + "models/";
	const string Config::scene_path = Config::game_res_path + "scenes/";
#else
	const string Config::engine_res_path = "../../../../../../myEngine/res/";
	const string Config::game_res_path = "../../../../../../res/";
	const string Config::model_path = Config::game_res_path + "models/";
	const string Config::scene_path = Config::game_res_path + "scenes/";
#endif

GameScene * GameScene::gs = NULL;


bool GameScene::init(){

    m_mainCharacter = Character::Create("model_1");
    if(m_mainCharacter == NULL)
        return false;
	m_camera = new Camera(glm::vec3(4.0f, 7.0f, 5.0f));
    //glDeleteVertexArrays(1, &vao);
    //glDeleteBuffers(1, &vbo);

    return true;
}
void GameScene::onDestroy()
{
    if(m_mainCharacter)
    {
        delete(m_mainCharacter);
        m_mainCharacter = NULL;
    }
    if(m_camera)
    {
        delete(m_camera);
        m_camera = NULL;
    }
}

void GameScene::render()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(m_mainCharacter)
    {
        m_mainCharacter->render();
    }
}

void GameScene::tick(float delta)
{
    render();
}