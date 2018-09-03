#include "RenderObject.h"

#include "StringDef.h"
#include "Material.h"

#include "bgfx/bgfx.h"

namespace myEngine
{
	RenderObject::RenderObject()
	:m_material(nullptr)
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
		else if (attr_str == "NORMAL")
			return bgfx::Attrib::Normal;
		else if (attr_str == "NORMAL")
			return bgfx::Attrib::Normal;
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

				const char* _from = reinterpret_cast<const char*>(&_buffer.data.data()[_bufferView.byteOffset + _accessor.byteOffset + _attr_size * i]);
				char*		_to = &buffer_data[i * _vertexSize + _attr_offset];

				myEngine::strcpy(_to, _attr_size, _from);
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

		const char* index_buffer_data = reinterpret_cast<const char*>(&_buffer.data.data()[_bufferView.byteOffset + _accessor.byteOffset]);
		int			index_buffer_size = _accessor.count * _accessor.ByteStride(_bufferView);
		m_ibh = bgfx::createIndexBuffer(bgfx::copy(index_buffer_data, index_buffer_size));
	}

	void RenderObject::_createProgram(const tinygltf::Primitive & primitive, const tinygltf::Model & model)
	{
		m_material = new Material;
		m_material->setProgram("pbr_gltf.vs", "pbr_gltf.fs");
	}
	
	void RenderObject::init(const tinygltf::Primitive& primitive, const tinygltf::Model& model)
	{
		_createVertexBuffer(primitive, model);
		_createIndexBuffer(primitive, model);
		_createProgram(primitive, model);
	}

	void RenderObject::draw()
	{
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_EQUAL);
		bgfx::setIndexBuffer(m_ibh);
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::submit(0, m_material->getProgram());




		// use the shader program
		m_shader->use();
		Matrix4 projection = m_camera->GetProjectMatrix();
		Matrix4 view = m_camera->GetViewMatrix();

		Matrix4 scaleM;
		scaleM.initWithScale(Vector3(m_scale));
		Matrix4 rotateM;
		rotateM.initWithRotate(m_rotateVec, m_theta);
		Matrix4 transM;
		transM.initWithTranslate(Vector3(0.0f));

		Matrix4 model = scaleM * rotateM * transM;


		m_shader->setMat4("model", model);
		m_shader->setMat4("view", view);
		m_shader->setMat4("projection", projection);
		m_shader->setVec3("cameraWorldPos", m_camera->Position);
		m_shader->setVec3("fogcolor", m_fogpara.color);
		m_shader->setVec3("fogpara", m_fogpara.start, m_fogpara.end, m_fogpara.intensity);

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			stringstream ss;
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++;
			else if (name == "texture_specular")
				ss << specularNr++;
			else if (name == "texture_normal")
				ss << normalNr++;
			else if (name == "texture_height")
				ss << heightNr++;

			number = ss.str();

			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);

		}
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}
}
