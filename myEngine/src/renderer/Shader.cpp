#include "Shader.h"

#include <string>
#include <iostream>
#include <assert.h>

#include "HelperFunc.h"

#include "bgfx/bgfx.h"
#include "brtshaderc.h"

namespace myEngine
{

    int Shader::UNIFORM_MAX_NUM = 32;
    
    Shader::Shader()
        :m_vertex_shader(BGFX_INVALID_HANDLE)
        , m_fragment_shader(BGFX_INVALID_HANDLE)
        , m_program(BGFX_INVALID_HANDLE)
    {

    
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

    void Shader::init(const char* vs_path, const char* fs_path, const char* varying_path, const char* vs_defines, const char* fs_defines)
    {
        const bgfx::Memory* _vs_men = shaderc::compileShader(shaderc::ST_VERTEX, vs_path, vs_defines, varying_path);
        m_vertex_shader = bgfx::createShader(_vs_men);
        if (!bgfx::isValid(m_vertex_shader))
        {
            std::cout << "Error while compiling vertex shader: " << vs_path << std::endl;
            return;
        }
        const bgfx::Memory* _fs_men = shaderc::compileShader(shaderc::ST_FRAGMENT, fs_path, fs_defines, varying_path);
        m_fragment_shader = bgfx::createShader(_fs_men);
        if (!bgfx::isValid(m_fragment_shader))
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
        m_uniform_name.reserve(num1 + num2);
        m_uniform_info.reserve(num1 + num2);
        for (std::vector<bgfx::UniformHandle>::iterator it = m_uniform.begin(); it != m_uniform.end(); it++)
        {
            bgfx::UniformInfo info;
            bgfx::getUniformInfo((*it), info);
            int index = it - m_uniform.begin();
            const std::string& name = std::string(info.name);
            m_uniform_name.push_back(name);
            m_uniform_idx[name] = index;
            m_uniform_info.push_back(info);
        }
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

    void Shader::setTexture(int stage, const std::string & name, const bgfx::TextureHandle & th) const
    {
        bgfx::UniformHandle _handle = _getUniformByName(name);
        bgfx::setTexture(stage, _handle, th);
    }

    const std::vector<std::string>& Shader::getAllUniformNames() const
    {
        return m_uniform_name;
    }

    const bgfx::UniformInfo& Shader::getUniformInfo(const std::string& s) const
    {
        int idx = m_uniform_idx.at(s);
        assert(idx >= 0 && static_cast<size_t>(idx) < m_uniform_info.size());
        return m_uniform_info[idx];
    }

    bgfx::UniformHandle Shader::_getUniformByName(const std::string& name) const
    {
        bgfx::UniformHandle result = BGFX_INVALID_HANDLE;
        std::unordered_map<std::string, int>::const_iterator it = m_uniform_idx.find(name);
        if (it != m_uniform_idx.end() && it->second < static_cast<int>(m_uniform.size()))
            result = m_uniform[it->second];
        return result;
    }

}
