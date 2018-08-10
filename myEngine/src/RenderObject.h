#pragma once
#include "bgfx/bgfx.h"
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
	public:
		RenderObject();
		~RenderObject();

		static bgfx::AttribType::Enum	mapAttributeComponentType(int gltf_attr_comp_type);
		static bgfx::Attrib::Enum		mapAttributeType(const std::string& attr_str);
	
		void init(const tinygltf::Primitive& primitive, const tinygltf::Model* model);
		void draw();
	};
}
