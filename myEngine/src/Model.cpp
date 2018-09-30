#include "Model.h"

#include "RenderObject.h"
#include "Renderer.h"
#include "Vector3.h"
#include "Quaternion.h"

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
	Model::Model()
	:m_gltf_model(nullptr)
	{
	}

	Model::~Model()
	{
		if (m_gltf_model)
		{
			delete m_gltf_model;
			m_gltf_model = nullptr;
		}

		for (const RenderObject* r : m_render_objects)
		{
			delete(r);
		}
		m_render_objects.clear();
	}
	
	void Model::load(const std::string &path)
	{
		m_gltf_model = new tinygltf::Model();
		tinygltf::TinyGLTF loader;
		std::string err;
		bool r = loader.LoadASCIIFromFile(m_gltf_model, &err, path);
		if (!r) {
			printf("LoadASCIIFromFile Error: %s\n", err.c_str());
			return;
		}

		_createTextureHandle();

		int _id = m_gltf_model->defaultScene;
		const tinygltf::Scene& _scene = m_gltf_model->scenes[_id];
		for (int n : _scene.nodes)
		{
			_loadNodeTreeMesh(n);
		}
		for (const tinygltf::Animation& a : m_gltf_model->animations)
		{
			// TODO
		}
	}

	void Model::_loadNodeTreeMesh(int node_id)
	{
		const tinygltf::Node& _node = m_gltf_model->nodes[node_id];
		//mesh node
		if (_node.mesh != -1)
		{
			const tinygltf::Mesh& _mesh = m_gltf_model->meshes[_node.mesh];
			for (const tinygltf::Primitive& p : _mesh.primitives)
			{
				RenderObject* _render_object = new RenderObject();
				_render_object->init(this, p, *m_gltf_model);
				m_render_objects.push_back(_render_object);
			}
			//mesh has skin
			if (_node.skin != -1)
			{
				const tinygltf::Skin& _skin = m_gltf_model->skins[_node.skin];
				for (int j : _skin.joints)
				{
					const tinygltf::Node& _node_joint = m_gltf_model->nodes[j];

					Matrix4 _sm;
					if (!_node_joint.scale.empty())
					{
						Vector3 _sv(
							static_cast<float>(_node_joint.scale[0]),
							static_cast<float>(_node_joint.scale[1]),
							static_cast<float>(_node_joint.scale[2]));
						_sm.initWithScale(_sv);
					}

					Matrix4 _rm;
					if (!_node_joint.rotation.empty())
					{
						Quaternion _rq(
							static_cast<float>(_node_joint.rotation[0]),
							static_cast<float>(_node_joint.rotation[1]),
							static_cast<float>(_node_joint.rotation[2]),
							static_cast<float>(_node_joint.rotation[3]));
						_rm.initWithRotateQuaternion(_rq);
					}

					Matrix4 _tm;
					if (!_node_joint.translation.empty())
					{
						Vector3 _tv(
							static_cast<float>(_node_joint.translation[0]),
							static_cast<float>(_node_joint.translation[1]),
							static_cast<float>(_node_joint.translation[2]));
						_tm.initWithTranslate(_tv);
					}

					Matrix4 _joint_matrix = _sm * _rm * _tm;

					m_joint_matrixs.push_back(_joint_matrix);
				}
			}
		}
		//load children recursive
		if (!_node.children.empty())
		{
			for (int i : _node.children)
				_loadNodeTreeMesh(i);
		}
	}

	void Model::_createTextureHandle()
	{
		if (nullptr == m_gltf_model)
			return;

		for (tinygltf::Image& _img : m_gltf_model->images)
		{
			uint32_t _size = static_cast<uint32_t>(_img.width * _img.height * _img.component);
			const bgfx::Memory* _data = bgfx::copy(_img.image.data(), _size);

			bgfx::TextureFormat::Enum _format;
			if (_img.component == 3)
				_format = bgfx::TextureFormat::RGB8;
			else if (_img.component == 4)
				_format = bgfx::TextureFormat::RGBA8;
			else
				_format = bgfx::TextureFormat::Count;

			bgfx::TextureHandle _th = bgfx::createTexture2D(static_cast<uint16_t>(_img.width),
															static_cast<uint16_t>(_img.height),
															false,
															1,
															_format,
															0,
															_data);
			_img.image.clear();
			_img.image.reserve(0);
			m_textrue_handles.push_back(_th);
		}
	}

	bgfx::TextureHandle Model::getTextureHandle(int index)
	{
		if (index < 0)
			return BGFX_INVALID_HANDLE;
		if (index > static_cast<int>(m_textrue_handles.size()))
			return BGFX_INVALID_HANDLE;
		return m_textrue_handles[index];
	}

	int Model::getJointMatrixsNum() const
	{
		return static_cast<int>(m_joint_matrixs.size());
	}

	const Matrix4* Model::getJointMatrixsData() const
	{
		return m_joint_matrixs.data();
	}

	void Model::draw()
	{
		Renderer* _renderer = Renderer::getInstance();
		if (nullptr == _renderer)
			return;
		for (const RenderObject* r : m_render_objects)
		{
			_renderer->pushRenderObject(r);
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
