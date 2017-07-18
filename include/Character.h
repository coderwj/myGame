#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "MyEngineCore.h"
#include "Model.h"
#include <string>

using namespace std;

class Character
{
public:
	Character();
	~Character();

	bool init(string modelName, glm::vec3 position = glm::vec3(0.0f), glm::vec3 orientation = glm::vec3(1.0f, 0.0f, 0.0f))；
private:
	glm::vec3 m_position;//pos in world
	glm::vec3 m_orientation;// ori in world
	Model * m_model;
	string m_modelName;
};

#endif //__CHARACTER_H__