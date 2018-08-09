#include "Model.h"

#include "RenderObject.h"

#ifndef TINYGLTF_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#endif // TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION
extern "C"
{
#include "stb_image.h"
}
#include "bgfx/bgfx.h"

using namespace std;

namespace myEngine
{
	Model::Model(const std::string &path)
	:m_gltf_model(nullptr)
	{
		loadModel(path);
	}

	Model::~Model()
	{
		delete m_gltf_model;
		m_gltf_model = nullptr;

		std::vector<RenderObject*>::iterator it = m_render_objects.begin();
		for (; it != m_render_objects.end(); it++)
		{
			delete (*it);
		}
		m_render_objects.clear();
	}
	
	void Model::loadModel(const string &path)
	{
		m_gltf_model = new(tinygltf::Model);
		tinygltf::TinyGLTF gltf_ctx;
		std::string err;
		bool ret = false;
		ret = gltf_ctx.LoadASCIIFromFile(m_gltf_model, &err, path);
		if (!ret) {
			printf("Failed to parse glTF\n");
			if (!err.empty())
				printf("Err: %s\n", err.c_str());
		}
		copyTexture();

		std::vector<tinygltf::Mesh>::iterator it = m_gltf_model->meshes.begin();
		for (; it != m_gltf_model->meshes.end(); it++)
		{
			std::vector<tinygltf::Primitive>::iterator it2 = (*it).primitives.begin();
			for (; it2 != (*it).primitives.end(); it2++)
			{
				RenderObject* pRenderObject = new RenderObject();
				std::map<std::string, int>::iterator itAttr = (*it2).attributes.begin();
				for (; itAttr != (*it2).attributes.end(); itAttr++)
				{

				}
				pRenderObject->init();
				m_render_objects.push_back(pRenderObject);
			}
		}
	}

	void Model::copyTexture()
	{
		assert(nullptr != m_gltf_model);
		for (std::vector<tinygltf::Image>::iterator it = m_gltf_model->images.begin(); it != m_gltf_model->images.end(); it++)
		{
			tinygltf::Image& _image = *it;
			_image.image;
			const bgfx::Memory* _data = bgfx::copy(static_cast<void*>(&(_image.image)), static_cast<uint32_t>(_image.width * _image.width * 4));
			bgfx::TextureHandle textureHandle = bgfx::createTexture2D(static_cast<uint16_t>(_image.width), 
																	  static_cast<uint16_t>(_image.width),
																	  false,
																	  1,
																	  bgfx::TextureFormat::BGRA8,
																	  0,
																	  _data);
			_image.image.clear();
			_image.image.reserve(0);
			textrue_handles.push_back(textureHandle);
		}
	}

	unsigned int Model::TextureFromFile(const char * path, const std::string &directory, bool gamma)
	{
		string filename = string(path);
		filename = directory + "/" + filename;
		unsigned int textureID = 0;

		int width, height, nrComponents;
		unsigned char * data = stbi_load(filename.c_str(), &width, &height, &nrComponents, STBI_default);
		if (data)
		{
			GLenum format = GL_RGBA;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 2)
				format = GL_RGB;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			cout << "Texture failed to load, path: " << filename << endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(1 * sizeof(Vector3)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(Vector3)));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(Vector3)));

		glBindVertexArray(0);
	}

	void Draw(Shader &shader)
	{

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

};
}