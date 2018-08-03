#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <map>

#include "bgfx/bgfx.h"

namespace myEngine
{
	class Shader
	{
	public:
	
		Shader(const char* vertexPath, const char* fragmentPath);
	
		~Shader();

		void setUniform(const std::string &name, const void* values) const;

		void setUniform(const std::string &name, const void* values, unsigned int count) const;
	private:
		bgfx::UniformHandle _getUniformByName(const std::string& name) const;

		bgfx::ShaderHandle m_vertex_shader;
		bgfx::ShaderHandle m_fragment_shader;
		bgfx::ProgramHandle m_program;
		std::vector<bgfx::UniformHandle> m_uniform;
		std::map<std::string, int> m_uniform_idx;

		static int UNIFORM_MAX_NUM;
	};
}

#endif