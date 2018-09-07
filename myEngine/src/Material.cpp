#include "Material.h"

#include "Engine.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

namespace myEngine
{
	Material::Material()
	:m_shader(nullptr)
	,m_doubleSided(false)
	,m_enableEmissive(false)
	,m_emissiveFactor(Vector3(1.f))
	,m_emissiveTextureID(-1)
	,m_normalTextureID(-1)
	,m_baseColorTextureID(-1)
	,m_metallicFactor(0.f)
	,m_metallicRoughnessTextureID(-1)
	,m_roughnessFactor(-1)
	{
		m_baseColorFactor.insert(m_baseColorFactor.end(), 4, 0);
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

	void Material::initParams(const tinygltf::Material & material_info)
	{
		for (tinygltf::ParameterMap::const_iterator it = material_info.values.begin(); it != material_info.values.end(); it++)
		{
			if (it->first == "baseColorFactor")
			{
				m_baseColorFactor.clear();
				for (int i = 0; i < 4; i++)
				{
					m_baseColorFactor.push_back(it->second.ColorFactor()[i]);
				}
			}
			else if (it->first == "baseColorTexture")
			{
				m_baseColorTextureID = it->second.TextureIndex();
			}
			else if (it->first == "metallicFactor")
			{
				m_metallicFactor = it->second.Factor();
			}
			else if (it->first == "metallicRoughnessTexture")
			{
				m_metallicRoughnessTextureID = it->second.TextureIndex();
			}
			else if (it->first == "roughnessFactor")
			{
				m_roughnessFactor = it->second.Factor();
			}
		}

		for (tinygltf::ParameterMap::const_iterator it = material_info.additionalValues.begin(); it != material_info.additionalValues.end(); it++)
		{
			bool _enableEmissive = false;
			if (it->first == "doubleSided")
			{
				m_doubleSided = it->second.bool_value;
			}
			else if (it->first == "emissiveFactor")
			{
				_enableEmissive = true;
				m_emissiveFactor = Vector3(it->second.ColorFactor()[0], it->second.ColorFactor()[1], it->second.ColorFactor()[2]);
			}
			else if (it->first == "emissiveTexture")
			{
				_enableEmissive = true;
				m_emissiveTextureID = it->second.TextureIndex();
			}
			else if (it->first == "normalTexture")
			{
				m_normalTextureID = it->second.TextureIndex();
			}
		}

	}

	void Material::bindUniforms(Model* _model)
	{
		if (nullptr == _model)
			return;
		if (nullptr == m_shader)
			return;

		// use the shader program
		myEngine::Camera* _camera = myEngine::Engine::getInstance()->getMaincCamera();
		Vector3& _camera_pos = _camera->getPosition();
		const Matrix4& _projection = _camera->GetProjectMatrix();
		const Matrix4& _view = _camera->GetViewMatrix();
		Matrix4 _m;
		Matrix4 u_MVPMatrix = _m * _view * _projection;


		std::vector<std::string> _uniform_names = m_shader->getAllUniformNames();

		for (std::vector<std::string>::iterator it = _uniform_names.begin(); it != _uniform_names.end(); it++)
		{
			bgfx::UniformInfo _info = m_shader->getUniformInfo(*it);
			if (_info.type == bgfx::UniformType::Int1) // Texture
			{
				bgfx::TextureHandle _th = _model->getTextureHandle(_info.num);
				m_shader->setTexture(_info.name, _th);
			}
			else// if (_info.type == bgfx::UniformType::Vec4)
			{
				if (*it == "u_MVPMatrix")
				{
					m_shader->setUniform(*it, static_cast<void*>(&u_MVPMatrix));
				}
				else if (*it == "u_ModelMatrix")
				{
					m_shader->setUniform(*it, static_cast<void*>(&_m));
				}
				else if (*it == "u_NormalMatrix")
				{
					m_shader->setUniform(*it, static_cast<void*>(&_m));
				}
				else if (*it == "u_Camera")
				{
					m_shader->setUniform(*it, static_cast<void*>(&_camera_pos));
				}
				else if (*it == "u_LightDirection")
				{
					float _lightDirection[4] = { 1.f, 1.f, 1.f, 1.f };
					m_shader->setUniform(*it, static_cast<void*>(_lightDirection));
				}
				else if (*it == "u_LightColor")
				{
					float _lightColor[4] = { 1.f, 1.f, 1.f, 1.f };
					m_shader->setUniform(*it, static_cast<void*>(_lightColor));
				}
				else if (*it == "u_NormalScale")
				{
					float _normalScale[4] = { 1.f, 1.f, 1.f, 1.f };
					m_shader->setUniform(*it, static_cast<void*>(_normalScale));
				}
				else if (*it == "u_EmissiveFactor")
				{
					float _emissiveFactor[4] = { m_emissiveFactor.x, m_emissiveFactor.y, m_emissiveFactor.z, 1.f };
					m_shader->setUniform(*it, static_cast<void*>(&_emissiveFactor));
				}
				else if (*it == "u_BaseColorFactor")
				{
					m_shader->setUniform(*it, static_cast<void*>(&m_baseColorFactor));
				}
				else if (*it == "u_MetallicRoughnessValues")
				{
					float _metallicRoughnessValues[4] = { m_metallicFactor, m_roughnessFactor, 1.f, 1.f };
					m_shader->setUniform(*it, static_cast<void*>(_metallicRoughnessValues));
				}
			}
		}
	}


}
