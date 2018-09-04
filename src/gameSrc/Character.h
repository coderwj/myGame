#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Vector3.h"
#include <string>
using namespace std;
using namespace myEngine;

namespace myEngine
{
	class Shader;
	class Model;
}

namespace myGame
{
	class Character
	{
	public:
		Character();
		~Character();
		static Character * Create(string modelName, Vector3 position = Vector3(0.0f), Vector3 orientation = Vector3(1.0f, 0.0f, 0.0f));
		void setPosition(Vector3 pos){m_position = pos;}
		Vector3 getPosition(){return m_position;}
		void setOrientation(Vector3 ori){m_orientation = ori;}
		Vector3 getOrientation(){return m_orientation;}
	
		void render();
	private:
		Vector3 m_position;//pos in world
		Vector3 m_orientation;// ori in world
		string m_modelName;
	
		Model * m_model;
	
		float m_scale;
		float m_theta;
		Vector3 m_rotateVec;
	
		bool init(string modelName, Vector3 position = Vector3(0.0f), Vector3 orientation = Vector3(1.0f, 0.0f, 0.0f));
	};
}

#endif //__CHARACTER_H__