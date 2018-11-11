#include "Character.h"

#include "Engine.h"
#include "Camera.h"
#include "Model.h"

#include "config.h"

#include "tinyxml2.h"

using namespace myEngine;

namespace myGame
{
	Character::Character():
	m_modelName(""),
	m_modelId(-1),
	m_scale(1.0f),
	m_theta(0.0f)
	{
	    m_position = Vector3(0.0f);
	    m_orientation = Vector3(1.0f, 0.0f, .0f);
	    m_rotateVec = Vector3(0.0f, 1.0f, 0.0f);
	}
	
	Character::~Character()
	{
		Engine* _engine = Engine::getInstance();
		if (_engine)
		{
			_engine->destroyModelById(m_modelId);
		}
	}
	
	bool Character::init(std::string modelName)
	{
		std::string model_str = "";
		std::string material_name = "";
		std::string model_config_path = Config::model_path + "modelConfig.xml";
	    tinyxml2::XMLDocument character_model_doc;
	    character_model_doc.LoadFile(model_config_path.c_str());
	    tinyxml2::XMLElement* character_model_element =
	    character_model_doc.FirstChildElement("modelConfig")->FirstChildElement("model");
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
				m_position.x = character_model_element->FirstChildElement("position")->FloatAttribute("x", 0.f);
				m_position.y = character_model_element->FirstChildElement("position")->FloatAttribute("y", 0.f);
				m_position.z = character_model_element->FirstChildElement("position")->FloatAttribute("z", 0.f);
	            m_rotateVec = Vector3(rotateX, rotateY, rotateZ);
	            break;
	        }
	    }
	    if(model_str == "")
	    {
	        return false;
	    }

		Engine* _engine = Engine::getInstance();
		if (nullptr == _engine)
		{
			return false;
		}
		m_modelId = _engine->createModel(Config::model_path + model_str);
		Model* _model = _engine->getModelById(m_modelId);
		if (_model)
		{
			Matrix4 _world_mat;
			_world_mat.initWithScale(Vector3(m_scale));

			//rotate
			Matrix4 _rotate_mat;
			_rotate_mat.initWithRotate(m_rotateVec, m_theta);
			_world_mat *= _rotate_mat;

			//translate
			_world_mat.translate(m_position);

			_model->setWorldMatrix(_world_mat);
		}

	    return true;
	}
	
	Character * Character::Create(std::string modelName)
	{
	    Character * cha = new Character();
	    if(cha->init(modelName))
	        return cha;
	    else
	        return NULL;
	}
	
	void Character::tick(int delta)
	{

	}
}