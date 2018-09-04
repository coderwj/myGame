#pragma once

#include "bgfx/bgfx.h"

#include <map>
#include <string>

namespace myEngine
{
	class Shader;
	class Material
	{
	public:
		Material();
		~Material();

		void setProgram(const char* vs_name, const char* fs_name);
		const Shader* getProgram();

		void setUniform(const char* name, const void* values);
	private:
		Shader* m_shader;
	};
}

