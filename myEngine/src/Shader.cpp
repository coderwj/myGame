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

		unsigned int vertex, fragment;

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
		uint16_t num = bgfx::getShaderUniforms(m_vertex_shader, &m_uniform[0], UNIFORM_MAX_NUM);
		bgfx::getShaderUniforms(m_fragment_shader, &m_uniform[num], UNIFORM_MAX_NUM - num);
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

	void Shader::use()
	{
		glUseProgram(ID);
	}

	void Shader::disuse()
	{
		glUseProgram(NULL);
	}

	void Shader::setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}



	void Shader::setUniform(float value)
	{
		bgfx::setUniform(_handle, &value);
	}

	void Shader::setValue(const float* values, unsigned int count)
	{
		bgfx::setUniform(_handle, &values[0], count);
	}

	void Shader::setMat4(const std::string &name, const Matrix4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat.m11));
	}

}