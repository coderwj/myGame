#include "Model.h"

#include "RenderObject.h"
#include "Renderer.h"
#include "Vector3.h"

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
				pRenderObject->init(this, *it2, *m_gltf_model);
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
			const bgfx::Memory* _data = bgfx::copy(reinterpret_cast<char*>(&(_image.image[0])), static_cast<uint32_t>(_image.width * _image.width * _image.component));
			bgfx::TextureFormat::Enum _format = bgfx::TextureFormat::Count;
			if (_image.component == 3)
				_format = bgfx::TextureFormat::RGB8;
			else if (_image.component == 4)
				_format = bgfx::TextureFormat::RGBA8;
			bgfx::TextureHandle textureHandle = bgfx::createTexture2D(static_cast<uint16_t>(_image.width), 
																	  static_cast<uint16_t>(_image.width),
																	  false,
																	  1,
																	  _format,
																	  0,
																	  _data);
			_image.image.clear();
			_image.image.reserve(0);
			m_textrue_handles.push_back(textureHandle);
		}
	}

	bgfx::TextureHandle Model::getTextureHandle(int index)
	{
		if (index >= 0 && index < static_cast<int>(m_textrue_handles.size()))
		{
			return m_textrue_handles[index];
		}
		return BGFX_INVALID_HANDLE;
	}

	void Model::draw()
	{
		Renderer* _renderer = Renderer::getInstance();
		if (nullptr == _renderer)
			return;
		std::vector<RenderObject*>::iterator it = m_render_objects.begin();
		for (; it != m_render_objects.end(); it++)
		{
			_renderer->pushRenderObject(*it);
		}
	}

	float Model::getScale() const
	{
		return m_scale;
	}

	void Model::setScale(float val)
	{
		m_scale = val;
	}

	Matrix4 Model::getModelMatrix()
	{
		Matrix4 _scale_mat;
		_scale_mat.initWithScale(Vector3(m_scale));
		Matrix4 _rotate_mat;
		Matrix4 _trans_mat;
		return _scale_mat * _rotate_mat * _trans_mat;
	}

}
