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
	
}