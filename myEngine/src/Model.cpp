#include "Model.h"

#include "RenderObject.h"

#ifndef TINYGLTF_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#endif // TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
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
				pRenderObject->init(*it2, *m_gltf_model);
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

	void Model::draw()
	{
		std::vector<RenderObject*>::iterator it = m_render_objects.begin();
		for (; it != m_render_objects.end(); it++)
		{
			(*it)->draw();
		}
	}
}
