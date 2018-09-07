#include "RenderObject.h"

#include "Engine.h"
#include "StringDef.h"
#include "Material.h"
#include "Shader.h"
#include "Matrix4.h"

#include "bgfx/bgfx.h"

namespace myEngine
{
	RenderObject::RenderObject()
	:m_model(nullptr)
	,m_material(nullptr)
	,m_vbh(BGFX_INVALID_HANDLE)
	,m_ibh(BGFX_INVALID_HANDLE)
	{
	
	}
	
	RenderObject::~RenderObject()
	{
		if (bgfx::isValid(m_vbh))
			bgfx::destroy(m_vbh);
		if (bgfx::isValid(m_ibh))
			bgfx::destroy(m_ibh);
		if (m_material)
			delete m_material;
	}

	bgfx::AttribType::Enum RenderObject::mapAttributeComponentType(int gltf_attr_comp_type)
	{
		if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_BYTE)
			return bgfx::AttribType::Uint8;

		else if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
			return bgfx::AttribType::Uint8;

		else if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_SHORT)
			return bgfx::AttribType::Uint8;

		else if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
			return bgfx::AttribType::Uint8;

		else if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_INT)
			return bgfx::AttribType::Int16;

		else if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
			return bgfx::AttribType::Int16;

		else if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_FLOAT)
			return bgfx::AttribType::Float;

		else if (gltf_attr_comp_type == TINYGLTF_COMPONENT_TYPE_DOUBLE)
			return bgfx::AttribType::Float;

		else
			// Unknown componenty type
			return bgfx::AttribType::Count;
	}

	bgfx::Attrib::Enum RenderObject::mapAttributeType(const std::string& attr_str)
	{
		if (attr_str == "POSITION")
			return bgfx::Attrib::Position;
		else if (attr_str == "NORMAL")
			return bgfx::Attrib::Normal;
		else if (attr_str == "TANGENT")
			return bgfx::Attrib::Tangent;
		else if (attr_str == "TEXCOORD_0")
			return bgfx::Attrib::TexCoord0;
		else if (attr_str == "NORMAL")
			return bgfx::Attrib::Normal;
		else
			return bgfx::Attrib::Count;
	}

	bool RenderObject::_cmpByValue(const std::pair<std::string, int>& p1, const std::pair<std::string, int>& p2)
	{
		return p1.second < p2.second;
	}

	void RenderObject::_createVertexBuffer(const tinygltf::Primitive & primitive, const tinygltf::Model & model)
	{
		// sort attributes
		std::vector<std::pair<std::string, int> > _attributes(primitive.attributes.begin(), primitive.attributes.end());
		std::sort(_attributes.begin(), _attributes.end(), RenderObject::_cmpByValue);


		// create vertex buffer
		int _vertexNum = 0;
		bgfx::VertexDecl _dec;
		_dec.begin();
		std::vector<std::pair<std::string, int> >::const_iterator it = _attributes.begin();
		for (; it != _attributes.end(); it++)
		{
			int _index = it->second;
			const tinygltf::Accessor& _accessor = model.accessors[_index];
			_dec.add(mapAttributeType(it->first), tinygltf::GetTypeSizeInBytes(_accessor.type), mapAttributeComponentType(_accessor.componentType));
			if (mapAttributeType(it->first) == bgfx::Attrib::Position)
				_vertexNum = _accessor.count;
		}
		_dec.end();

		const int _vertexSize = _dec.getStride();
		char* buffer_data = new char[_vertexNum * _vertexSize];
		for (int i = 0; i < _vertexNum; i++)
		{
			for (it = _attributes.begin(); it != _attributes.end(); it++)
			{
				const tinygltf::Accessor&	 _accessor = model.accessors[it->second];
				const tinygltf::BufferView& _bufferView = model.bufferViews[_accessor.bufferView];
				const tinygltf::Buffer&		_buffer = model.buffers[_bufferView.buffer];

				int _attr_offset = _dec.getOffset(mapAttributeType(it->first));
				int _attr_size = _accessor.ByteStride(_bufferView);

				const char* _from = reinterpret_cast<const char*>(&_buffer.data[_bufferView.byteOffset + _accessor.byteOffset + _attr_size * i]);
				char*		_to = &buffer_data[i * _vertexSize + _attr_offset];

				::memcpy(_to, _from, _attr_size);
			}
		}
		m_vbh = bgfx::createVertexBuffer(bgfx::copy(buffer_data, _vertexNum * _vertexSize), _dec);
		delete[] buffer_data;
	}

	void RenderObject::_createIndexBuffer(const tinygltf::Primitive & primitive, const tinygltf::Model & model)
	{
		// create index buffer
		const tinygltf::Accessor&	_accessor = model.accessors[primitive.indices];
		const tinygltf::BufferView& _bufferView = model.bufferViews[_accessor.bufferView];
		const tinygltf::Buffer&		_buffer = model.buffers[_bufferView.buffer];

		const char* index_buffer_data = reinterpret_cast<const char*>(&_buffer.data[_bufferView.byteOffset + _accessor.byteOffset]);
		int			index_buffer_size = _accessor.count * _accessor.ByteStride(_bufferView);
		m_ibh = bgfx::createIndexBuffer(bgfx::copy(index_buffer_data, index_buffer_size), BGFX_BUFFER_INDEX32);
	}

	void RenderObject::_createProgram(const tinygltf::Primitive & primitive, const tinygltf::Model & model)
	{
		m_material = new Material;
		m_material->setProgram("pbr_gltf_vs.bin", "pbr_gltf_fs.bin");


		const tinygltf::Material& _material = model.materials[primitive.material];

		m_material->initParams(_material);
		//if(_material.values.find("baseColorFactor") != _material.values.end())
	}
	
	void RenderObject::init(Model* modelptr, const tinygltf::Primitive& primitive, const tinygltf::Model& model)
	{
		m_model = modelptr;
		_createVertexBuffer(primitive, model);
		_createIndexBuffer(primitive, model);
		_createProgram(primitive, model);
	}

	void RenderObject::draw()
	{
		if (nullptr == m_material)
			return;
		const myEngine::Shader* _shader = m_material->getProgram();
		if (nullptr == _shader)
			return;

		bgfx::setState(BGFX_STATE_DEFAULT);

		bgfx::setIndexBuffer(m_ibh);
		bgfx::setVertexBuffer(0, m_vbh);

		m_material->bindUniforms(m_model);

		bgfx::submit(0, _shader->getProgramHandle());
	}
}
