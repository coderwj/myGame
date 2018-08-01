#ifndef SHADER_H
#define SHADER_H

#include "myEngineCore.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "bx/readerwriter.h"
#include "bgfx/bgfx.h"

namespace myEngine
{
	class Shader
	{
	public:
	
	    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	    {
	        std::string vertexCode;
	        std::string fragmentCode;
	
	        std::ifstream vShaderFile;
	        std::ifstream fShaderFile;
	
	        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
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

			const bgfx::Memory* memVs = bgfx::makeRef(vShaderCode, vertexCode.size());
			const bgfx::Memory* memFs = bgfx::makeRef(fShaderCode, fragmentCode.size());

			bgfx::ShaderHandle memVsh = bgfx::createShader(memVs);
			bgfx::ShaderHandle memFsh = bgfx::createShader(memFs);

			if (isValid(memVsh))
			{
				std::cout << "Error while compiling vertex shader: " << vertexPath <<std::endl;
				return;
			}

			if (isValid(memFsh))
			{
				std::cout << "Error while compiling fragment shader: " << fragmentPath << std::endl;
				return;
			}

			_program = bgfx::createProgram(
				bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
				, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
				, true
			);


			// Create shaders.
			_vsh = bgfx::createShader(memVsh);
			_fsh = bgfx::createShader(memFsh);

			// Create bgfx program.
			_program = bgfx::createProgram(_vsh, _fsh, true);

			if (!bgfx::isValid(_program))
			{
				GP_WARN("Error while creating bgfx program with shaders [%s], [%s], [%s].", vshPath, fshPath, defines);
				return false;
			}

			// Query uniforms from shaders.
			getUniformsFromShader(_vsh);
			getUniformsFromShader(_fsh);

	        vertex = glCreateShader(GL_VERTEX_SHADER);
	        glShaderSource(vertex, 1, &vShaderCode, NULL);
	        glCompileShader(vertex);
	        checkCompileErrors(vertex, "VERTEX");
	        // fragment Shader
	        fragment = glCreateShader(GL_FRAGMENT_SHADER);
	        glShaderSource(fragment, 1, &fShaderCode, NULL);
	        glCompileShader(fragment);
			checkCompileErrors(fragment, "FRAGMENT");
			// if geometry shader is given, compile geometry shader
			unsigned int geometry;
			if(geometryPath != nullptr)
			{
				const char * gShaderCode = geometryCode.c_str();
				geometry = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometry, 1, &gShaderCode, NULL);
				glCompileShader(geometry);
				checkCompileErrors(geometry, "GEOMETRY");
			}
	        // shader Program
	        ID = glCreateProgram();
	        glAttachShader(ID, vertex);
	        glAttachShader(ID, fragment);
			if(geometryPath != nullptr)
				glAttachShader(ID, geometry);
	        glLinkProgram(ID);
	        checkCompileErrors(ID, "PROGRAM");
	
	        glDeleteShader(vertex);
	        glDeleteShader(fragment);
			if(geometryPath != nullptr)
				glDeleteShader(geometry);
	    }
	
		Shader(Shader &shader)
		{
			ID = 0;
		}
	
	    ~Shader()
	    {
	        glDeleteProgram(ID);
	    }
	
	    void use()
	    {
	        glUseProgram(ID);
	    }
	
	    void disuse()
	    {
	        glUseProgram(NULL);
	    }
	
	    void setBool(const std::string &name, bool value) const
	    {
	        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	    }
	
	    void setInt(const std::string &name, int value) const
	    {
	        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	    }
	
	    void setFloat(const std::string &name, float value) const
	    {
	        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	    }
	
	    // void setVec2(const std::string &name, const glm::vec2 &value) const
	    // {
	    //     glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	    // }
	    void setVec2(const std::string &name, float x, float y) const
	    {
	        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	    }
	
	    void setVec3(const std::string &name, const Vector3 &value) const
	    {
	        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &(value.x));
	    }
	    void setVec3(const std::string &name, float x, float y, float z) const
	    {
	        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	    }
	
	    // void setVec4(const std::string &name, const glm::vec4 &value) const
	    // {
	    //     glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	    // }
	    void setVec4(const std::string &name, float x, float y, float z, float w)
	    {
	        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	    }
	
	    // void setMat2(const std::string &name, const glm::mat2 &mat) const
	    // {
	    //     glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	    // }
	
	    // void setMat3(const std::string &name, const glm::mat3 &mat) const
	    // {
	    //     glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	    // }
	
	    void setMat4(const std::string &name, const Matrix4 &mat) const
	    {
	        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat.m11));
	    }
	
	private:
		bgfx::ProgramHandle m_program;
		bgfx::UniformHandle m_uniform;
	};
}

#endif