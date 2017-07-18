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
	static Character * Create(string modelName, glm::vec3 position = glm::vec3(0.0f), glm::vec3 orientation = glm::vec3(1.0f, 0.0f, 0.0f));
	void setPosition(glm::vec3 pos){m_position = pos;}
	glm::vec3 getPosition(){return m_position;}
	void setOrientation(glm::vec3 ori){m_orientation = ori;}
	glm::vec3 getOrientation(){return m_orientation;}

	void render();
private:
	glm::vec3 m_position;//pos in world
	glm::vec3 m_orientation;// ori in world
	Model * m_model;
	string m_modelName;

	bool init(string modelName, glm::vec3 position = glm::vec3(0.0f), glm::vec3 orientation = glm::vec3(1.0f, 0.0f, 0.0f));
};

#endif //__CHARACTER_H__