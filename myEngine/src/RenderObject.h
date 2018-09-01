#pragma once
#include "bgfx/bgfx.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

namespace myEngine
{
	class Material;

	class RenderObject
	{
	private:
		Material*	m_material;
		bgfx::VertexBufferHandle	m_vbh;
		bgfx::IndexBufferHandle		m_ibh;

		static bool _cmpByValue(const std::pair<std::string, int>& p1, const std::pair<std::string, int>& p2);

		void _createVertexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
		void _createIndexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
		void _createProgram(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
	public:
		RenderObject();
		~RenderObject();

		static bgfx::AttribType::Enum	mapAttributeComponentType(int gltf_attr_comp_type);
		static bgfx::Attrib::Enum		mapAttributeType(const std::string& attr_str);
	
		void init(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
		void draw();
	};
}
