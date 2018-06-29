#pragma once

#include "MyEngineCore.h"
#include <string>

class Shader;

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	bool init(const char* modelName);
	void render();
private:
	Shader * m_shader;

	float m_scale;
	float m_theta;		//rotate by Y
	Vector3 m_position;//pos in world
};
