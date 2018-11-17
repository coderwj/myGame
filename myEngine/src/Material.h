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
	class Node;
	class Material
	{
	public:
		Material();
		~Material();

		void setProgram(const char* vs_name, const char* fs_name);
		const Shader* getProgram();

		void setUniform(const char* name, const void* values);

		void initParams(const tinygltf::Material& material_info, const tinygltf::Model & model);

		void bindUniforms(Model* _model, Node* _node);

		void setHasSkin(bool v) { m_hasSkin = v; }

		void setHasUv(bool v) { m_hasUv = v; }

		void setHasTangent(bool v) { m_hasTangent = v; }

	private:
		std::string _getVertexShaderDefines();
		std::string _getFragmentShaderDefines();

	private:
		Shader* m_shader;

		//info from normal/occlusion/emissive values
		bool m_doubleSided;

		Vector3 m_emissiveFactor;
		int m_emissiveTextureID;

		float m_occlusionStrengh;
		int m_occlusionTextureID;

		int m_normalTextureID;


		//info from PBR metal/roughness workflow
		std::vector<float> m_baseColorFactor;
		int m_baseColorTextureID;

		float m_metallicFactor;
		int m_metallicRoughnessTextureID;
		float m_roughnessFactor;

		bool m_hasSkin;
		bool m_hasUv;
		bool m_hasTangent;
	};
}

