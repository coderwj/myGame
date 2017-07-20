#include "Character.h"

#include "GameScene.h"
#include "Camera.h"

#include "config.h"

using namespace std;

Character::Character():
m_model(NULL),
m_modelName("")
{
	m_position = glm::vec3(0.0f);
	m_orientation = glm::vec3(1.0f, 0.0f, .0f);
}

Character::~Character()
{
	if(m_model)
	{
		delete(m_model);
		m_model = NULL;
	}
	if(m_shader)
	{
		delete(m_shader);
		m_shader = NULL;
	}
}

bool Character::init(string modelName, glm::vec3 position, glm::vec3 orientation)
{
	string model_str = "";
	string material_name = "";
	string model_config_path = Config::model_path + "modelConfig.xml";
	tinyxml2::XMLDocument character_model_doc;
	character_model_doc.LoadFile(model_config_path.c_str());
	tinyxml2::XMLElement* character_model_element = 
	character_model_doc.FirstChildElement("modelconfig")->FirstChildElement("model");
	for (;;character_model_element = character_model_element->NextSiblingElement("model")) {
        if(character_model_element == NULL)
            break;
        if(modelName == character_model_element->FirstChildElement("name")->GetText())
        {
            model_str = character_model_element->FirstChildElement("path")->GetText();
            material_name = character_model_element->FirstChildElement("material")->GetText();
            break;
        }
    }
    if(model_str == "")
    {
    	//todo log info
    	return false;
    }
    m_model = new Model(Config::model_path + model_str);

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

    return true;
}

Character * Character::Create(string modelName, glm::vec3 position, glm::vec3 orientation)
{
	Character * cha = new Character();
	if(cha->init(modelName, position, orientation))
		return cha;
	else
		return NULL;
}

void Character::render()
{
	// use the shader program
    m_shader->use();
    GameScene * gamescene = GameScene::getInstance();
    if(!gamescene)
        return;
    Camera * camera = gamescene->getCamera();
    if(!camera)
        return;
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), 4.0f / 3.0f, 0.1f, 1000.0f);
    glm::mat4 view = camera->GetViewMatrix();
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
}