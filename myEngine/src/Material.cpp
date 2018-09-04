#include "Material.h"

#include "Shader.h"

namespace myEngine
{
	Material::Material()
	:m_shader(nullptr)
	{
	}

	Material::~Material()
	{
		if (m_shader)
			delete m_shader;
	}

	void Material::setProgram(const char* vs_name, const char* fs_name)
	{
		if (m_shader != NULL)
			return;
		m_shader = new Shader(vs_name, fs_name);
	}

	const Shader* Material::getProgram()
	{
		return m_shader;
	}

	void Material::setUniform(const char * name, const void * values)
	{
		if (m_shader)
		{
			m_shader->setUniform(name, values);
		}
	}


}
