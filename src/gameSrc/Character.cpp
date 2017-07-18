#include "Character.h"
#include "Config.h"

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
}

bool Character::init(string modelName, glm::vec3 position, glm::vec3 orientation)
{
	string model_str = ""
	tinyxml2::XMLDocument character_model_doc;
	character_model_doc:LoadFile(Config::model_path + "modelConfig.xml");
	tinyxml2::XMLElement* character_model_element = 
	character_model_doc.FirstChildElement("modelconfig")->FirstChildElement("model");
	for (;;character_model_element = character_model_element->NextSiblingElement("model")) {
        if(character_model_element == NULL)
            break;
        if(modelName == materialElement->FirstChildElement("name")->GetText())
        {
            model_str = materialElement->FirstChildElement("path")->GetText();
            break;
        }
    }
    if(model_str == "")
    {
    	//todo log info
    	return false;
    }
    m_model = new Model(Config::model_path + model_str);
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

}