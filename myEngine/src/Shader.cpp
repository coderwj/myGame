#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "bgfx/bgfx.h"

namespace myEngine
{

	int Shader::UNIFORM_MAX_NUM = 32;
	
	Shader::Shader(const char* vertexPath, const char* fragmentPath)
		:m_vertex_shader(BGFX_INVALID_HANDLE)
		, m_fragment_shader(BGFX_INVALID_HANDLE)
		, m_program(BGFX_INVALID_HANDLE)
	{
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		const bgfx::Memory* memVs = bgfx::copy(vShaderCode, vertexCode.size());
		const bgfx::Memory* memFs = bgfx::copy(fShaderCode, fragmentCode.size());


		// Create shaders.
		bgfx::ShaderHandle m_vertex_shader = bgfx::createShader(memVs);
		bgfx::ShaderHandle m_fragment_shader = bgfx::createShader(memFs);

		if (isValid(m_vertex_shader))
		{
			std::cout << "Error while compiling vertex shader: " << vertexPath << std::endl;
			return;
		}

		if (isValid(m_fragment_shader))
		{
			std::cout << "Error while compiling fragment shader: " << fragmentPath << std::endl;
			return;
		}
		// Create bgfx program.
		m_program = bgfx::createProgram(m_vertex_shader, m_fragment_shader, true);

		if (!bgfx::isValid(m_program))
		{
			std::cout << "Error while creating bgfx program with shaders" << vertexPath << "," << fragmentPath << "." << std::endl;
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

	void Shader::setUniform(const std::string &name , const void* values) const
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
