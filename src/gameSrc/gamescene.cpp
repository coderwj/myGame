
#include <string>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

#include "GameScene.h"
#include "SkyBox.h"

#include "Character.h"
#include "Camera.h"
#include "luaClientPort.h"

#include "Config.h"
#include "Shader.h"
#include "Model.h"
#include "tinyxml2.h"

#include "FbxSdkHelper.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "lua_tinker.h"

#include <thread>
#include <sstream>

using namespace std;
using namespace myEngine;


namespace myGame
{
	GameScene * GameScene::gs = NULL;
	
	
	bool GameScene::init(){
	
		m_state = luaL_newstate();
		luaopen_base(m_state);
		luaL_openlibs(m_state);
		tolua__open(m_state);
		string luafile = Config::lua_path + "dofile.lua";
		lua_tinker::dofile(m_state, luafile.c_str());
	
		lua_tinker::call<void>(m_state, "LuaGameMgr", "InitGame");
	
		if (Config::GetIsShaderTest())
		{
			return true;
		}
	
	    string fbxFile = Config::model_path + "character2/Maskboy.FBX";
	    printFbxFileData(fbxFile.c_str());
	
		m_skyBox = new SkyBox;
		m_skyBox->init("");
	
	    string modelName = "scene_3";
	    loadScene(modelName.c_str());
	    m_mainCharacter = Character::Create("model_3");
	    m_mainCharacter->setPosition(Vector3(20.0f, 1.0f, 10.0f));
	
	    Character * character = Character::Create("model_4");
	    character->setPosition(Vector3(18.0f, 1.0f, 11.0f));
	    m_characters.push_back(character);
	    if(m_mainCharacter == NULL)
	        return false;
	
		m_camera = new Camera;
		resetCameraPos();
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
	    for (vector<Character *>::iterator it = m_characters.begin(); it != m_characters.end();)
	    {
	        if(*it)
	        {
	            delete(*it);
	            it = m_characters.erase(it);
	        }
	        else
	        {
	            it++;
	        }
	    }
	}
	
	void GameScene::render()
	{
	    glEnable(GL_DEPTH_TEST);
		if (Config::GetIsShaderTest())
		{
			static bool hasInit = false;
			/*static float vertices[48] = {
				-320.0f, -240.0f, 0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				255.0f,  0.0f,    0.0f,        //color
	
	
				320.0f,  -240.0f, 0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				255.0f,  0.0f,    0.0f,        //color
	
				320.0f,  240.0f,  0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				255.0f,  0.0f,    0.0f,        //color
	
				-320.0f, 240.0f,  0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				255.0f,  0.0f,    0.0f        //color
			};*/
			static float vertices[48] = {
				-0.9f, -0.9f, 0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				255.0f,  0.0f,    0.0f,        //color
	
	
				0.9f,  -0.9f, 0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				255.0f,  0.0f,    0.0f,        //color
	
				0.9f,  0.9f,  0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				0.0f,  255.0f,    0.0f,        //color
	
				-0.9f, 0.9f,  0.0f,        //pos
				0.0f,    0.0f,    1.0f,        //normal
				0.0f,    0.0f,    0.0f,        //textcoord
				0.0f,  255.0f,    0.0f        //color
			};
			static unsigned int indices[6] = {
				0, 1, 2,
			    2, 3, 0
			};
			static unsigned int VAO, VBO, EBO;
			if (!hasInit)
			{
				hasInit = true;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);
	
				glBindVertexArray(VAO);
	
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)0);
	
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(3 * sizeof(float)));
	
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(6 * sizeof(float)));
	
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(9 * sizeof(float)));
	
				glBindVertexArray(0);
			}
			string temp_vs_path = Config::engine_res_path + "shader/" + "test.vs";
			string temp_fs_path = Config::engine_res_path + "shader/" + "test.fs";
			static Shader * testShader = new Shader(temp_vs_path.c_str(), temp_fs_path.c_str());
	
			testShader->use();
	
			GameScene * gamescene = GameScene::getInstance();
			if (!gamescene)
				return;
			Camera * camera = gamescene->getCamera();
			if (!camera)
				return;
			Matrix4 projection = camera->GetProjectMatrix();
			Matrix4 view;
			view.initWithLookAt(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f), Vector3(0.0f, 1.0f, 0.0f));
	
			Matrix4 scaleM;
			scaleM.initWithScale(Vector3(1.0f));
			Matrix4 rotateM;
			rotateM.initWithRotate(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
			Matrix4 transM;
			transM.initWithTranslate(Vector3(0.0f));
	
			Matrix4 model = scaleM * rotateM * transM;
			testShader->setMat4("model", model);
			testShader->setMat4("view", view);
			testShader->setMat4("projection", projection);
	        testShader->setFloat("time", m_nowTime);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			return;
		}
	    _renderScene();
	    if(m_mainCharacter)
	    {
	        m_mainCharacter->render();
	    }
	    for (std::vector<Character *>::iterator it = m_characters.begin(); it != m_characters.end(); it++)
	    {
	        if(*it)
	        {
	            (*it)->render();
	        }
	    }
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