#include "Shader.h"

#include <string>
#include <iostream>

#include "config.h"
#include "HelperFunc.h"

#include "bgfx/bgfx.h"

namespace myEngine
{

	int Shader::UNIFORM_MAX_NUM = 32;
	
	Shader::Shader(const char* vs_name, const char* fs_name)
		:m_vertex_shader(BGFX_INVALID_HANDLE)
		, m_fragment_shader(BGFX_INVALID_HANDLE)
		, m_program(BGFX_INVALID_HANDLE)
	{

		string vs_path = Config::shader_bin_path + vs_name;
		int vs_bin_size = HelperFunc::getFileSize(vs_path.c_str());
		char * vs_bin = new char[vs_bin_size];
		HelperFunc::LoadFromFile(vs_path.c_str(), vs_bin, vs_bin_size);
		const bgfx::Memory* _vs_men = bgfx::makeRef(vs_bin, vs_bin_size);
		bgfx::ShaderHandle m_vertex_shader = bgfx::createShader(_vs_men);
		if (isValid(m_vertex_shader))
		{
			std::cout << "Error while compiling vertex shader: " << vs_path << std::endl;
			return;
		}

		string fs_path = Config::shader_bin_path + fs_name;
		int fs_bin_size = HelperFunc::getFileSize(fs_path.c_str());
		char * fs_bin = new char[fs_bin_size];
		HelperFunc::LoadFromFile(fs_path.c_str(), fs_bin, fs_bin_size);
		const bgfx::Memory* _fs_men = bgfx::makeRef(fs_bin, fs_bin_size);
		bgfx::ShaderHandle _fsh = bgfx::createShader(_fs_men);
		if (isValid(m_fragment_shader))
		{
			std::cout << "Error while compiling fragment shader: " << fs_path << std::endl;
			return;
		}

		// Create bgfx program.
		m_program = bgfx::createProgram(m_vertex_shader, m_fragment_shader, true);
		if (!bgfx::isValid(m_program))
		{
			std::cout << "Error while creating bgfx program with shaders" << vs_path << "," << fs_path << "." << std::endl;
			return;
		}

		// Query uniforms from shaders.
		m_uniform.resize(UNIFORM_MAX_NUM);
		uint16_t num1 = bgfx::getShaderUniforms(m_vertex_shader, &m_uniform[0], UNIFORM_MAX_NUM);
		uint16_t num2 = bgfx::getShaderUniforms(m_fragment_shader, &m_uniform[num1], UNIFORM_MAX_NUM - num1);
		m_uniform.resize(num1 + num2);
		for (std::vector<bgfx::UniformHandle>::iterator it = m_uniform.begin(); it != m_uniform.end(); it++)
		{
			bgfx::UniformInfo info;
			bgfx::getUniformInfo((*it), info);
			m_uniform_idx[std::string(info.name)] = it->idx;
		}
	}
	Shader::~Shader()
	{
		if (bgfx::isValid(m_program))
			bgfx::destroy(m_program);
		if (bgfx::isValid(m_fragment_shader))
			bgfx::destroy(m_fragment_shader);
		if (bgfx::isValid(m_vertex_shader))
			bgfx::destroy(m_vertex_shader);
	}

	void Shader::setUniform(const std::string &name, const void* values) const
	{
		bgfx::UniformHandle _handle = _getUniformByName(name);
		bgfx::setUniform(_handle, values);
	}

	void Shader::setUniform(const std::string &name, const void* values, unsigned int count) const
	{
		bgfx::UniformHandle _handle = _getUniformByName(name);
		bgfx::setUniform(_handle, values, count);
	}

	bgfx::UniformHandle Shader::_getUniformByName(const std::string & name) const
	{
		std::map<std::string, int>::const_iterator it = m_uniform_idx.find(name);
		if (it != m_uniform_idx.end() && it->second < static_cast<int>(m_uniform.size()))
			return m_uniform[it->second];

		return bgfx::UniformHandle(BGFX_INVALID_HANDLE);
	}

}
