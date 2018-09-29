#include "Character.h"

#include "Engine.h"
#include "Camera.h"
#include "Model.h"

#include "config.h"

#include "tinyxml2.h"

using namespace std;
using namespace myEngine;

namespace myGame
{
	Character::Character():
	m_modelName(""),
	m_model(NULL),
	m_scale(1.0f),
	m_theta(0.0f)
	{
	    m_position = Vector3(0.0f);
	    m_orientation = Vector3(1.0f, 0.0f, .0f);
	    m_rotateVec = Vector3(0.0f, 1.0f, 0.0f);
	}
	
	Character::~Character()
	{
	    if(m_model)
	    {
	        delete(m_model);
	        m_model = NULL;
	    }
	}
	
	bool Character::init(string modelName)
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
	            m_scale = character_model_element->FirstChildElement("scale")->FloatText(1.0f);
	            m_theta = character_model_element->FirstChildElement("rotateTheta")->FloatText(0.0f);
	            float rotateX = character_model_element->FirstChildElement("rotateX")->FloatText(0.0f);
	            float rotateY = character_model_element->FirstChildElement("rotateY")->FloatText(1.0f);
	            float rotateZ = character_model_element->FirstChildElement("rotateZ")->FloatText(0.0f);
	            m_rotateVec = Vector3(rotateX, rotateY, rotateZ);
	            break;
	        }
	    }
	    if(model_str == "")
	    {
	        return false;
	    }
	    m_model = new Model(Config::model_path + model_str);
		m_model->setScale(m_scale);

	    return true;
	}
	
	Character * Character::Create(string modelName)
	{
	    Character * cha = new Character();
	    if(cha->init(modelName))
	        return cha;
	    else
	        return NULL;
	}
	
	void Character::tick(int delta)
	{
		m_model->draw();
	}
}