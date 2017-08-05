
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

#include "MyEngineCore.h"
#include "GameScene.h"

#include "Character.h"
#include "Camera.h"

#include "Config.h"
#include "Shader.h"
#include "Model.h"
#include "tinyxml2.h"

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
	string modelname = "scene_1";
    loadScene(modelname);
    m_mainCharacter = Character::Create("model_1");
    if(m_mainCharacter == NULL)
        return false;
	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 800.0f));
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
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //renderScene();
    if(m_mainCharacter)
    {
        m_mainCharacter->render();
    }
}

void GameScene::renderScene()
{
    // use the shader program
    m_shader->use();
    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), 4.0f / 3.0f, 0.1f, 1000.0f);
    glm::mat4 view = m_camera->GetViewMatrix();
    glm::vec3 model_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 model = glm::mat4(1.0);
    model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, model_pos);

    m_shader->setMat4("model", model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    m_model->Draw(*m_shader);
    //m_shader->disuse();
}

void GameScene::tick(float delta)
{
    render();
}

void GameScene::loadScene(string scenename)
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
        assert(true);
    }
    string vs_path = Config::engine_res_path + "shader/" + vs_name;
    string fs_path = Config::engine_res_path + "shader/" + fs_name;

    m_shader = new Shader(vs_path.c_str(), fs_path.c_str());

}