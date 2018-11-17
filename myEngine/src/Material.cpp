#include "Material.h"

#include <iostream>

#include "Engine.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Node.h"

namespace myEngine
{
	Material::Material()
	:m_shader(nullptr)
	,m_doubleSided(false)
	,m_emissiveFactor(Vector3(0.f))
	,m_emissiveTextureID(-1)
	, m_occlusionStrengh(1.f)
	, m_occlusionTextureID(-1)
	,m_normalTextureID(-1)
	,m_baseColorTextureID(-1)
	,m_metallicFactor(1.f)
	,m_metallicRoughnessTextureID(-1)
	,m_roughnessFactor(1.f)
	,m_hasSkin(false)
	,m_hasUv(false)
	,m_hasTangent(false)
	{
		m_baseColorFactor.insert(m_baseColorFactor.end(), 4, 1.f);
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
		m_shader = new Shader();
		const std::string& vs_defines = _getVertexShaderDefines();
		const std::string& fs_defines = _getFragmentShaderDefines();
		m_shader->initDynamicShader(vs_name, fs_name, vs_defines.c_str(), fs_defines.c_str());
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

	void Material::initParams(const tinygltf::Material & material_info, const tinygltf::Model & model)
	{
		for (tinygltf::ParameterMap::const_iterator it = material_info.values.begin(); it != material_info.values.end(); it++)
		{
			if (it->first.compare("baseColorFactor") == 0)
			{
				m_baseColorFactor.clear();
				for (int i = 0; i < 4; i++)
				{
					float v = static_cast<float>(it->second.ColorFactor()[i]);
					m_baseColorFactor.push_back(v);
				}
			}
			else if (it->first.compare("baseColorTexture") == 0)
			{
				m_baseColorTextureID = model.textures[it->second.TextureIndex()].source;
			}
			else if (it->first.compare("metallicFactor") == 0)
			{
				m_metallicFactor = static_cast<float>(it->second.Factor());
			}
			else if (it->first.compare("metallicRoughnessTexture") == 0)
			{
				m_metallicRoughnessTextureID = model.textures[it->second.TextureIndex()].source;
			}
			else if (it->first.compare("roughnessFactor") == 0)
			{
				m_roughnessFactor = static_cast<float>(it->second.Factor());
			}
			else
			{
				//std::cout << "Error values!" << std::endl;
			}
		}

		for (tinygltf::ParameterMap::const_iterator it = material_info.additionalValues.begin(); it != material_info.additionalValues.end(); it++)
		{
			if (it->first.compare("doubleSided") == 0)
			{
				m_doubleSided = it->second.bool_value;
			}
			else if (it->first.compare("emissiveFactor") == 0)
			{
				float x = static_cast<float>(it->second.ColorFactor()[0]);
				float y = static_cast<float>(it->second.ColorFactor()[1]);
				float z = static_cast<float>(it->second.ColorFactor()[2]);
				m_emissiveFactor = Vector3(x, y, z);
			}
			else if (it->first.compare("emissiveTexture") == 0)
			{
				m_emissiveTextureID = model.textures[it->second.TextureIndex()].source;
			}
			else if (it->first.compare("occlusionTexture") == 0)
			{
				m_occlusionTextureID = model.textures[it->second.TextureIndex()].source;
			}
			else if (it->first.compare("normalTexture") == 0)
			{
				m_normalTextureID = model.textures[it->second.TextureIndex()].source;
			}
			else
			{
				//std::cout << "Error additionalValues!" << std::endl;
			}
		}

	}

	void Material::bindUniforms(Model* _model, Node* _node)
	{
		if (nullptr == _model)
			return;
		if (nullptr == _node)
			return;
		if (nullptr == m_shader)
			return;

		myEngine::Camera* _camera = myEngine::Engine::getInstance()->getMaincCamera();
		if (nullptr == _camera)
			return;

		Matrix4 _mat_model;
		_mat_model.initWithScaleRotateTranslate(_node->getScale(), _node->getRotate(), _node->getTranslate());
		const Matrix4& _mat_world = _mat_model * _model->getWorldMatrix();


		std::vector<std::string> _uniform_names = m_shader->getAllUniformNames();

		int _stage = 0;
		for (std::vector<std::string>::iterator it = _uniform_names.begin(); it != _uniform_names.end(); it++)
		{
			bgfx::UniformInfo _info = m_shader->getUniformInfo(*it);
			if (_info.type == bgfx::UniformType::Int1) // Texture
			{
				int texture_id = -1;
				if ((*it).compare("u_BaseColorSampler") == 0)
				{
					texture_id = m_baseColorTextureID;
				}
				else if ((*it).compare("u_MetallicRoughnessSampler") == 0)
				{
					texture_id = m_metallicRoughnessTextureID;
				}
				else if ((*it).compare("u_EmissiveSampler") == 0)
				{
					texture_id = m_emissiveTextureID;
				}
				else if ((*it).compare("u_OcclusionSampler") == 0)
				{
					texture_id = m_occlusionTextureID;
				}
				else if ((*it).compare("u_NormalSampler") == 0)
				{
					texture_id = m_normalTextureID;
				}

				if (texture_id >= 0)
				{
					bgfx::TextureHandle _th = _model->getTextureHandle(texture_id);
					m_shader->setTexture(_stage, _info.name, _th);
					_stage++;
				}
				else
				{
					//std::cout << "Error texture_id!" << std::endl;
				}
			}
			else// if (_info.type == bgfx::UniformType::Vec4)
			{
				if ((*it).compare("u_VPMatrix") == 0)
				{
					const Matrix4& _mat_proj = _camera->GetProjectMatrix();
					const Matrix4& _mat_view = _camera->GetViewMatrix();
					Matrix4 u_VPMatrix = _mat_view * _mat_proj;
					m_shader->setUniform(*it, static_cast<const void*>(&u_VPMatrix));
				}
				else if ((*it).compare("u_WorldMatrix") == 0)
				{
					m_shader->setUniform(*it, static_cast<const void*>(&_mat_world));
				}
				else if ((*it).compare("u_NormalMatrix") == 0)
				{
					m_shader->setUniform(*it, static_cast<const void*>(&_mat_world));
				}
				else if ((*it).compare("u_Camera") == 0)
				{
					const Vector3& _camera_pos = _camera->getPosition();
					m_shader->setUniform(*it, static_cast<const void*>(&_camera_pos));
				}
				else if ((*it).compare("u_LightDirection") == 0)
				{
					//float _lightDirection[4] = { -1.f, -1.f, -1.f, 0.f };
					float _lightDirection[4] = { 1.f, 1.f, 1.f, 0.f };
					m_shader->setUniform(*it, static_cast<const void*>(_lightDirection));
				}
				else if ((*it).compare("u_LightColor") == 0)
				{
					float _lightColor[4] = { 1.f, 1.f, 1.f, 1.f };
					m_shader->setUniform(*it, static_cast<const void*>(_lightColor));
				}
				else if ((*it).compare("u_NormalScale") == 0)
				{
					float _normalScale[4] = { 1.f, 1.f, 1.f, 1.f };
					m_shader->setUniform(*it, static_cast<const void*>(_normalScale));
				}
				else if ((*it).compare("u_EmissiveFactor") == 0)
				{
					float _emissiveFactor[4] = { m_emissiveFactor.x, m_emissiveFactor.y, m_emissiveFactor.z, 0.f };
					m_shader->setUniform(*it, static_cast<const void*>(&_emissiveFactor));
				}
				else if ((*it).compare("u_OcclusionStrength") == 0)
				{
					float _occlusionFactor[4] = { m_occlusionStrengh, 0.f, 0.f, 0.f };
					m_shader->setUniform(*it, static_cast<const void*>(&_occlusionFactor));
				}
				else if ((*it).compare("u_BaseColorFactor") == 0)
				{
					m_shader->setUniform(*it, static_cast<const void*>(m_baseColorFactor.data()));
				}
				else if ((*it).compare("u_MetallicRoughnessValues") == 0)
				{
					float _metallicRoughnessValues[4] = { m_metallicFactor, m_roughnessFactor, 0.f, 0.f };
					m_shader->setUniform(*it, static_cast<const void*>(_metallicRoughnessValues));
				}
				else if ((*it).compare("u_JointMatrixs") == 0)
				{
					const void* _data = static_cast<const void*>(_model->getJointMatrixsData());
					m_shader->setUniform(*it, _data, Skeleton::MAX_JOINT_NUM);
				}
				else
				{
					std::cout << "Error uniform name!" << std::endl;
				}
			}
		}
	}

	std::string Material::_getVertexShaderDefines()
	{
		std::string result = "";
		if (true)
			result += "HAS_NORMALS;";
		if (m_hasTangent)
			result += "HAS_TANGENTS;";
		if (m_hasUv)
			result += "HAS_UV;";
		if (m_hasSkin)
			result += "HAS_SKIN;";
		return result;
	}

	std::string Material::_getFragmentShaderDefines()
	{
		std::string result = "";
		if (m_baseColorTextureID != -1)
			result += "HAS_BASECOLORMAP;";
		if (m_hasTangent)
			result += "HAS_TANGENTS;";
		if (true)
			result += "HAS_NORMALS;";
		if (m_normalTextureID != -1)
			result += "HAS_NORMALMAP;";
		if (m_emissiveTextureID != -1)
			result += "HAS_EMISSIVEMAP;";
		if (m_occlusionTextureID != -1)
			result += "HAS_OCCLUSIONMAP;";
		if (m_metallicRoughnessTextureID != -1)
			result += "HAS_METALROUGHNESSMAP;";
		return result;
	}


}
