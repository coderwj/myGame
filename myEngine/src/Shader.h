#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "bgfx/bgfx.h"

namespace myEngine
{
	class Shader
	{
	public:
	
		Shader();
		~Shader();

		void	initDynamicShader(const char* vs_name, const char* fs_name, const char * vs_defines, const char * fs_defines);

		void    setUniform(const std::string &name, const void* values) const;
		void    setUniform(const std::string &name, const void* values, unsigned int count) const;

		void	setTexture(int stage, const std::string &name, const bgfx::TextureHandle& th) const;

		const std::vector<std::string>& getAllUniformNames() const;
		const bgfx::UniformInfo& getUniformInfo(const std::string& s) const;

		//void    setUniform(const char* name, const void* values) const { setUniform(std::string(name), values); }
		//void    setUniform(const char* name, const void* values, unsigned int count) const { setUniform(std::string(name), values, count); }

		bgfx::ProgramHandle getProgramHandle() const { return m_program; }

	private:
		bgfx::UniformHandle                 _getUniformByName       (const std::string& name) const;

	private:
		bgfx::ShaderHandle                  m_vertex_shader;
		bgfx::ShaderHandle                  m_fragment_shader;
		bgfx::ProgramHandle                 m_program;
		std::vector<bgfx::UniformHandle>    m_uniform;
		std::vector<std::string>			m_uniform_name;
		std::unordered_map<std::string, int> m_uniform_idx;
		std::vector<bgfx::UniformInfo>		m_uniform_info;

		static int                          UNIFORM_MAX_NUM;
	};
}

#endif
