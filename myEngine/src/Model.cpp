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
	,m_render_object(nullptr)
	{
		loadModel(path);
	}

	Model::~Model()
	{
		delete m_gltf_model;
		m_gltf_model = nullptr;

		delete m_render_object;
		m_render_object = nullptr;
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

		m_render_object = new RenderObject();
		copyTexture();
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
}