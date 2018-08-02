#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>

#include "myEngineCore.h"

#include "bgfx/bgfx.h"

namespace myEngine
{
	class Shader
	{
	public:
	
		Shader(const char* vertexPath, const char* fragmentPath);
	
		~Shader();
	
		void use();
		void disuse();
	
		void setBool(const std::string &name, bool value) const;
	
		void setInt(const std::string &name, int value) const;

		void setValue(float value);

		void setValue(const float* values, unsigned int count);


	
		void setFloat(const std::string &name, float value) const;
		void setVec2(const std::string &name, float x, float y) const;
		void setVec3(const std::string &name, const Vector3 &value) const;
		void setVec3(const std::string &name, float x, float y, float z) const;
		void setVec4(const std::string &name, float x, float y, float z, float w) const;
	
		void setMat4(const std::string &name, const Matrix4 &mat) const;
	
	private:
		bgfx::ShaderHandle m_vertex_shader;
		bgfx::ShaderHandle m_fragment_shader;
		bgfx::ProgramHandle m_program;
		std::vector<bgfx::UniformHandle> m_uniform;

		static int UNIFORM_MAX_NUM;
	};
}

#endif