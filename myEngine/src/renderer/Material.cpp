#include "Material.h"

#include <iostream>

#include "Engine.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Node.h"
#include "config.h"

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
	,m_useIBL(false)
	{
		m_baseColorFactor.insert(m_baseColorFactor.end(), 4, 1.f);
	}

	Material::~Material()
	{
		if (m_shader)
			delete m_shader;
	}

	void Material::createShaderProgram()
	{
		std::string material_name = "pbr_gltf";
		if (!m_customerMaterialName.empty())
		{
			material_name = m_customerMaterialName;
		}
		std::string vs_path, fs_path, varying_path;
		Config::getShaderPathByMaterialName(material_name, vs_path, fs_path, varying_path);

		const std::string& vs_defines = _getVertexShaderDefines();
		const std::string& fs_defines = _getFragmentShaderDefines();

		if (m_shader != NULL)
			return;
		m_shader = new Shader();
		m_shader->init(vs_path.c_str(), fs_path.c_str(), varying_path.c_str(), vs_defines.c_str(), fs_defines.c_str());
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

		tinygltf::Value extraValues = material_info.extras;
		if (extraValues.Size() > 0)
		{
			tinygltf::Value useIBLValue = extraValues.Get("useibl");
			if (useIBLValue.IsInt() && useIBLValue.Get<int>() == 1)
			{
				setUseIBL(true);
			}

			tinygltf::Value customerMaterialValue = extraValues.Get("customer");
			if (customerMaterialValue.IsString() && customerMaterialValue.Get<std::string>() != "")
			{
				const std::string& _material = customerMaterialValue.Get<std::string>();
				m_customerMaterialName = _material;
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

		Engine * _engine = Engine::getInstance();
		if (nullptr == _engine)
			return;

		myEngine::Camera* _camera = myEngine::Engine::getInstance()->getMaincCamera();
		if (nullptr == _camera)
			return;

		const Matrix4& _mat_model = _node->generateGlobalMatrix();
		const Matrix4& _mat_world = _model->getWorldMatrix();


		const std::vector<std::string>& _uniform_names = m_shader->getAllUniformNames();

		int _stage = 0;
		for (const std::string& s : _uniform_names)
		{
			bgfx::UniformInfo _info = m_shader->getUniformInfo(s);
			if (_info.type == bgfx::UniformType::Sampler) // Texture
			{
				int texture_id = -1;
				if (s.compare("u_BaseColorSampler") == 0)
				{
					texture_id = m_baseColorTextureID;
				}
				else if (s.compare("u_MetallicRoughnessSampler") == 0)
				{
					texture_id = m_metallicRoughnessTextureID;
				}
				else if (s.compare("u_EmissiveSampler") == 0)
				{
					texture_id = m_emissiveTextureID;
				}
				else if (s.compare("u_OcclusionSampler") == 0)
				{
					texture_id = m_occlusionTextureID;
				}
				else if (s.compare("u_NormalSampler") == 0)
				{
					texture_id = m_normalTextureID;
				}
				else if (s.compare("u_DiffuseEnvSampler") == 0)
				{
					bgfx::TextureHandle _th = _engine->getDiffuseEnvTextureHandle();
					m_shader->setTexture(_stage, s, _th);
					_stage++;
				}
				else if (s.compare("u_SpecularEnvSampler") == 0)
				{
					bgfx::TextureHandle _th = _engine->getSpecularEnvTextureHandle();
					m_shader->setTexture(_stage, s, _th);
					_stage++;
				}
				else if (s.compare("u_brdfLUT") == 0)
				{
					bgfx::TextureHandle _th = _engine->getBrdfLUTTexture();
					m_shader->setTexture(_stage, s, _th);
					_stage++;
				}

				if (texture_id >= 0)
				{
					bgfx::TextureHandle _th = _model->getTextureHandle(texture_id);
					m_shader->setTexture(_stage, s, _th);
					_stage++;
				}
				else
				{
					//std::cout << "Error texture_id!" << std::endl;
				}
			}
			else// if (_info.type == bgfx::UniformType::Vec4)
			{
				if (s.compare("u_VPMatrix") == 0)
				{
					const Matrix4& _mat_proj = _camera->GetProjectMatrix();
					const Matrix4& _mat_view = _camera->GetViewMatrix();
					Matrix4 u_VPMatrix = _mat_view * _mat_proj;
					m_shader->setUniform(s, static_cast<const void*>(&u_VPMatrix));
				}
				else if (s.compare("u_WorldMatrix") == 0)
				{
					m_shader->setUniform(s, static_cast<const void*>(&_mat_world));
				}
				else if (s.compare("u_ModelMatrix") == 0)
				{
					m_shader->setUniform(s, static_cast<const void*>(&_mat_model));
				}
				else if (s.compare("u_NormalMatrix") == 0)
				{
					m_shader->setUniform(s, static_cast<const void*>(&_mat_model));
				}
				else if (s.compare("u_Camera") == 0)
				{
					const Vector3& _camera_pos = _camera->getPosition();
					m_shader->setUniform(s, static_cast<const void*>(&_camera_pos));
				}
				else if (s.compare("u_LightDirection") == 0)
				{
					//float _lightDirection[4] = { -1.f, -1.f, -1.f, 0.f };
					float _lightDirection[4] = { 1.f, 1.f, 1.f, 0.f };
					m_shader->setUniform(s, static_cast<const void*>(_lightDirection));
				}
				else if (s.compare("u_LightColor") == 0)
				{
					float _lightColor[4] = { 1.f, 1.f, 1.f, 1.f };
					m_shader->setUniform(s, static_cast<const void*>(_lightColor));
				}
				else if (s.compare("u_NormalScale") == 0)
				{
					float _normalScale[4] = { 1.f, 1.f, 1.f, 1.f };
					m_shader->setUniform(s, static_cast<const void*>(_normalScale));
				}
				else if (s.compare("u_EmissiveFactor") == 0)
				{
					float _emissiveFactor[4] = { m_emissiveFactor.x, m_emissiveFactor.y, m_emissiveFactor.z, 0.f };
					m_shader->setUniform(s, static_cast<const void*>(&_emissiveFactor));
				}
				else if (s.compare("u_OcclusionStrength") == 0)
				{
					float _occlusionFactor[4] = { m_occlusionStrengh, 0.f, 0.f, 0.f };
					m_shader->setUniform(s, static_cast<const void*>(&_occlusionFactor));
				}
				else if (s.compare("u_BaseColorFactor") == 0)
				{
					m_shader->setUniform(s, static_cast<const void*>(m_baseColorFactor.data()));
				}
				else if (s.compare("u_MetallicRoughnessValues") == 0)
				{
					float _metallicRoughnessValues[4] = { m_metallicFactor, m_roughnessFactor, 0.f, 0.f };
					m_shader->setUniform(s, static_cast<const void*>(_metallicRoughnessValues));
				}
				else if (s.compare("u_JointMatrixs") == 0)
				{
					const void* _data = static_cast<const void*>(_model->getJointMatrixsData(_node));
					m_shader->setUniform(s, _data, Skeleton::MAX_JOINT_NUM);
				}
				else if (s.compare("u_Time") == 0)
				{
					float _time = _engine->getTotalTime() / 1000.f;
					float _timeVec[4] = { _time, 0.f, 0.f, 0.f };
					m_shader->setUniform(s, static_cast<const void*>(&_timeVec));
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
		if (m_useIBL)
			result += "USE_IBL;";
		return result;
	}


}
