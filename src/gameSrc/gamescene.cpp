#include "GameScene.h"

#include "Config.h"
#include "tinyxml2.h"

#include "SkyBox.h"
#include "Camera.h"
#include "Model.h"

#include <string>

using namespace std;
using namespace myEngine;


namespace myGame
{
	GameScene * GameScene::m_pGameScene = NULL;
	
	
	GameScene * GameScene::getInstance()
	{
		if (m_pGameScene == NULL)
			m_pGameScene = new GameScene();
		return m_pGameScene;
	}

	void GameScene::destroyInstance()
	{
		if (m_pGameScene)
		{
			m_pGameScene->onDestroy();
			delete(m_pGameScene);
			m_pGameScene = NULL;
		}
	}

	bool GameScene::init(){

	    string fbxFile = Config::model_path + "character2/Maskboy.FBX";
	    printFbxFileData(fbxFile.c_str());
	
		m_skyBox = new SkyBox;
		m_skyBox->init("2/darkcity_");
	
	    string sceneName = "scene_3";
	    loadScene(sceneName.c_str());
	    m_mainCharacter = Character::Create("model_3");
	    m_mainCharacter->setPosition(Vector3(20.0f, 1.0f, 10.0f));
	
	    Character * character = Character::Create("model_4");
	    character->setPosition(Vector3(18.0f, 1.0f, 11.0f));
	    m_characters.push_back(character);
	    if(nullptr == m_mainCharacter)
	        return false;
	
	    return true;
	}
	GameScene::GameScene() :
		m_skyBox(nullptr),
		m_model(nullptr),
		m_scale(1.0f),
		m_theta(0.0f)
	{
		m_rotateVec = Vector3(0.0f, 1.0f, 0.0f);
		m_fogpara.color = Vector3(1.0f, 1.0f, 1.0f);
		m_fogpara.start = 10.f;
		m_fogpara.end = 100.f;
		m_fogpara.intensity = 1.f;
	}

	GameScene::~GameScene()
	{

	}

	void GameScene::onDestroy()
	{
	}
	
	void GameScene::render()
	{
		_renderScene();
	}
	
	void GameScene::_renderScene()
	{
		m_skyBox->render();
	
	    // use the shader program
	    m_shader->use();
	    Matrix4 projection = m_camera->GetProjectMatrix();
	    Matrix4 view = m_camera->GetViewMatrix();
	
	    Matrix4 scaleM;
	    scaleM.initWithScale(Vector3(m_scale));
	    Matrix4 rotateM;
	    rotateM.initWithRotate(m_rotateVec, m_theta);
	    Matrix4 transM;
	    transM.initWithTranslate(Vector3(0.0f));
	
	    Matrix4 model = scaleM * rotateM * transM;
	
	
	    m_shader->setMat4("model", model);
	    m_shader->setMat4("view", view);
	    m_shader->setMat4("projection", projection);
		m_shader->setVec3("cameraWorldPos", m_camera->Position);
		m_shader->setVec3("fogcolor", m_fogpara.color);
		m_shader->setVec3("fogpara", m_fogpara.start, m_fogpara.end, m_fogpara.intensity);
	
	    m_model->Draw(*m_shader);
	    m_shader->disuse();
	}
	
	void GameScene::tick(float delta)
	{
		m_deltaTime = delta;
		lua_tinker::call<void>(m_state, "LuaGameMgr", "Tick", delta);
	    m_nowTime += delta;
	}
	
	void GameScene::loadScene(const char* sceneName)
	{
	    string model_str = "";
	    string material_name = "";
	    string scene_config_path = Config::scene_path + "sceneConfig.xml";
	    tinyxml2::XMLDocument scene_model_doc;
	    scene_model_doc.LoadFile(scene_config_path.c_str());
	    tinyxml2::XMLElement* scene_model_element =
	    scene_model_doc.FirstChildElement("sceneconfig")->FirstChildElement("scene");
	    for (;;scene_model_element = scene_model_element->NextSiblingElement("scene")) {
	        if(scene_model_element == NULL)
	            break;
	        if(scenename == scene_model_element->FirstChildElement("name")->GetText())
	        {
	            model_str = scene_model_element->FirstChildElement("path")->GetText();
	            material_name = scene_model_element->FirstChildElement("material")->GetText();
				m_scale = scene_model_element->FirstChildElement("scale")->FloatText(1.0f);
				m_theta = scene_model_element->FirstChildElement("rotateTheta")->FloatText(0.0f);
				float rotateX = scene_model_element->FirstChildElement("rotateX")->FloatText(0.0f);
				float rotateY = scene_model_element->FirstChildElement("rotateY")->FloatText(1.0f);
				float rotateZ = scene_model_element->FirstChildElement("rotateZ")->FloatText(0.0f);
				m_rotateVec = Vector3(rotateX, rotateY, rotateZ);
	
				tinyxml2::XMLElement* fog_element = scene_model_element->FirstChildElement("fogpara");
				if(NULL != fog_element)
				{
					float red = fog_element->FirstChildElement("color")->FirstChildElement("red")->FloatText(m_fogpara.color.x);
					float green = fog_element->FirstChildElement("color")->FirstChildElement("green")->FloatText(m_fogpara.color.y);
					float blue = fog_element->FirstChildElement("color")->FirstChildElement("blue")->FloatText(m_fogpara.color.z);
					m_fogpara.color = Vector3(red, green, blue);
					m_fogpara.start = fog_element->FirstChildElement("start")->FloatText(m_fogpara.start);
					m_fogpara.end = fog_element->FirstChildElement("end")->FloatText(m_fogpara.end);
					m_fogpara.intensity = fog_element->FirstChildElement("intensity")->FloatText(m_fogpara.intensity);
				}
	            break;
	        }
	    }
	    if(model_str == "")
	    {
	        //todo log info
	        return;
	    }
	    m_model = new Model(Config::scene_path + model_str);
	
	    string shader_config_path = Config::engine_res_path + "shader/shaderConfig.xml";
	    tinyxml2::XMLDocument shader_doc;
	    shader_doc.LoadFile(shader_config_path.c_str());
	
	    string vs_name = "", fs_name = "";
	
	    tinyxml2::XMLElement* materialElement = shader_doc.FirstChildElement("shaderconfig")->FirstChildElement( "material" );
	    for (;;materialElement = materialElement->NextSiblingElement("material")) {
	        if(materialElement == NULL)
	        {
	            break;
	        }
	        if(material_name == materialElement->FirstChildElement("name")->GetText())
	        {
	            vs_name = materialElement->FirstChildElement("vertexshader")->GetText();
	            fs_name = materialElement->FirstChildElement("fragmentshader")->GetText();
	            break;
	        }
	    }
	    if(vs_name == "" || fs_name == "")
	    {
	        return;
	        //assert(true);
	    }
	    string vs_path = Config::engine_res_path + "shader/" + vs_name;
	    string fs_path = Config::engine_res_path + "shader/" + fs_name;
	
	    m_shader = new Shader(vs_path.c_str(), fs_path.c_str());
		//glCheckError();
	}
	
	void GameScene::changeScene(const char* sceneName)
	{
		if (m_shader)
		{
			delete m_shader;
			m_shader = NULL;
		}
		if (m_model)
		{
			delete m_model;
			m_model = NULL;
		}
		loadScene(sceneName);
	}

}