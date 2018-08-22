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
	
	void RenderObject::init(const tinygltf::Primitive& primitive, const tinygltf::Model* model)
	{
		bgfx::VertexDecl _dec;
		_dec.begin();
		std::map<std::string, int>::const_iterator it = primitive.attributes.begin();
		for (; it != primitive.attributes.end(); it++)
		{
				int _index = it->second;
				tinygltf::Accessor _acc = model->accessors[_index];
				_dec.add(mapAttributeType(it->first), tinygltf::GetTypeSizeInBytes(_acc.type), mapAttributeComponentType(_acc.componentType));
		}
		_dec.end();
		m_vbh = bgfx::createVertexBuffer(bgfx::copy(v_buf, v_size), _dec);
		m_ibh = bgfx::createIndexBuffer(bgfx::copy(ibuf, sizeof(uint16_t) * k));
	}

	void RenderObject::draw()
	{
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_EQUAL);
		bgfx::setIndexBuffer(m_ibh);
		bgfx::setVertexBuffer(0, m_vbh);
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
