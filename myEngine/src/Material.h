#pragma once

#include "bgfx/bgfx.h"
#include "tiny_gltf.h"

#include "Vector3.h"

#include <vector>
#include <map>
#include <string>

namespace myEngine
{
	class Shader;
	class Model;
	class Material
	{
	public:
		Material();
		~Material();

		void setProgram(const char* vs_name, const char* fs_name);
		const Shader* getProgram();

		void setUniform(const char* name, const void* values);

		void initParams(const tinygltf::Material& material_info);

		void bindUniforms(Model* _model);

	private:
		std::string _getVertexShaderDefines();
		std::string _getFragmentShaderDefines();

	private:
		Shader* m_shader;

		//info from normal/occlusion/emissive values
		bool m_doubleSided;

		bool m_enableEmissive;
		Vector3 m_emissiveFactor;
		int m_emissiveTextureID;

		int m_normalTextureID;


		//info from PBR metal/roughness workflow
		std::vector<float> m_baseColorFactor;
		int m_baseColorTextureID;

		float m_metallicFactor;
		int m_metallicRoughnessTextureID;
		float m_roughnessFactor;
	};
}

