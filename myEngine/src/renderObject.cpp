#include "RenderObject.h"

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
	
	void RenderObject::init(const tinygltf::Primitive& primitive, const tinygltf::Model* model)
	{
		std::vector<std::pair<std::string, int>> attributes_in_order(primitive.attributes.begin(), primitive.attributes.end());
		std::sort(attributes_in_order.begin(), attributes_in_order.end(), RenderObject::_cmpByValue);


		int vertexNum = 0;
		bgfx::VertexDecl _dec;
		_dec.begin();
		std::vector<std::pair<std::string, int>>::const_iterator it = attributes_in_order.begin();
		for (; it != attributes_in_order.end(); it++)
		{
			int _index = it->second;
			const tinygltf::Accessor& _accessor = model->accessors[_index];
			_dec.add(mapAttributeType(it->first), tinygltf::GetTypeSizeInBytes(_accessor.type), mapAttributeComponentType(_accessor.componentType));
			if (mapAttributeType(it->first) == bgfx::Attrib::Position)
				vertexNum = _accessor.count;
		}
		_dec.end();

		const int stride = _dec.getStride();
		const int buffer_size = vertexNum * stride;
		unsigned char* buffer_data = new unsigned char[buffer_size];
		for (int i = 0; i < vertexNum; i++)
		{
			for (it = attributes_in_order.begin(); it != attributes_in_order.end(); it++)
			{
				int _index = it->second;
				const tinygltf::Accessor& _accessor = model->accessors[_index];
				const tinygltf::BufferView& _bufferView = model->bufferViews[_accessor.bufferView];

				const std::vector<unsigned char>& _buffer = model->buffers[_bufferView.buffer].data;

				const void* buffer_data = static_cast<const void*>(model->buffers[_bufferView.buffer].data.data());
				_bufferView.buffer
			}
		}

		for (it = attributes_in_order.begin(); it != attributes_in_order.end(); it++)
		{
			int _index = it->second;
			const tinygltf::Accessor& _accessor = model->accessors[_index];
			const tinygltf::BufferView& _bufferView = model->bufferViews[_accessor.bufferView];

			const std::vector<unsigned char>& _buffer = model->buffers[_bufferView.buffer].data;

			const void* buffer_data = static_cast<const void*>(model->buffers[_bufferView.buffer].data.data());
			_bufferView.buffer
		}
		int buffer_size = model->buffers[buffer_id].data.size();
		m_vbh = bgfx::createVertexBuffer(bgfx::copy(buffer_data, buffer_size), _dec);

		int index_buffer_view_id = primitive.indices;
		const void* index_buffer_data = static_cast<const void*>(model->buffers[index_buffer_view_id].data.data());
		int index_buffer_size = model->buffers[index_buffer_view_id].data.size();
		m_ibh = bgfx::createIndexBuffer(bgfx::copy(index_buffer_data, index_buffer_size));
	}

	void RenderObject::draw()
	{
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_EQUAL);
		bgfx::setIndexBuffer(m_ibh);
		bgfx::setVertexBuffer(0, m_vbh, ,);
		bgfx::submit(0, m_program);




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
