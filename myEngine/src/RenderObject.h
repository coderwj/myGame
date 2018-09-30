#pragma once
#include "bgfx/bgfx.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

namespace myEngine
{
	class Material;
	class Model;

	class RenderObject
	{
	public:
		RenderObject();
		~RenderObject();

		static bgfx::AttribType::Enum	mapAttributeComponentType(int gltf_attr_comp_type);
		static bgfx::Attrib::Enum		mapAttributeType(const std::string& attr_str);

		void init(Model* modelptr, const tinygltf::Primitive& primitive, const tinygltf::Model& model);
		void draw() const;

	private:
		static bool _cmpByValue(const std::pair<std::string, int>& p1, const std::pair<std::string, int>& p2);

		void _createVertexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
		void _createIndexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
		void _createProgram(const tinygltf::Primitive& primitive, const tinygltf::Model& model);

	private:
		Model* m_model;
		Material*	m_material;
		bgfx::VertexBufferHandle	m_vbh;
		bgfx::IndexBufferHandle		m_ibh;
	};
}
