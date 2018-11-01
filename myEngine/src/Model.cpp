#include "Model.h"

#include "RenderObject.h"
#include "Renderer.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Animation.h"
#include "Engine.h"
#include "Node.h"

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
		Engine* _engine = Engine::getInstance();
		if (nullptr == _engine)
		{
			return;
		}
		for (const tinygltf::Animation& a : m_gltf_model->animations)
		{
			Animation *_anim = new Animation();
			for (const tinygltf::AnimationChannel& c : a.channels)
			{
				int node_index = c.target_node;
				KeyChain _chain;
				const std::string& path = c.target_path;
				//"translation", "rotation", "scale", "weights"
				if (path == "translation")
				{
					_chain.setType(KEY_CHAIN_TYPE_V3);
				}
				else if (path == "rotation")
				{
					_chain.setType(KEY_CHAIN_TYPE_V4);
				}
				else if (path == "scale")
				{
					_chain.setType(KEY_CHAIN_TYPE_V3);
				}
				else if (path == "weights")
				{
					//TODO
				}

				const tinygltf::AnimationSampler& s = a.samplers[c.sampler];
				if (s.interpolation == "LINEAR" || s.interpolation == "")//default: "LINEAR"
				{
					_chain.setAccType(KEY_CHAIN_ACCELERATE_LINEAR);
				}
				else if (s.interpolation == "STEP")
				{
					_chain.setAccType(KEY_CHAIN_ACCELERATE_STEP);
				}
				else if (s.interpolation == "CATMULLROMSPLINE")
				{
					_chain.setAccType(KEY_CHAIN_ACCELERATE_CATMULLROMSPLINE);
				}
				else if (s.interpolation == "CUBICSPLINE")
				{
					_chain.setAccType(KEY_CHAIN_ACCELERATE_CUBICSPLINE);
				}

				//time accessor
				const tinygltf::Accessor&	_acc_t	= m_gltf_model->accessors[s.input];
				const tinygltf::BufferView& _view_t = m_gltf_model->bufferViews[_acc_t.bufferView];
				const tinygltf::Buffer&		_buf_t	= m_gltf_model->buffers[_view_t.buffer];

				//value accessor
				const tinygltf::Accessor&	_acc_v	= m_gltf_model->accessors[s.output];
				const tinygltf::BufferView& _view_v = m_gltf_model->bufferViews[_acc_v.bufferView];
				const tinygltf::Buffer&		_buf_v	= m_gltf_model->buffers[_view_v.buffer];

				const float* value_buffer_data = reinterpret_cast<const float*>(&_buf_v.data[_view_v.byteOffset + _acc_v.byteOffset]);


				const float* time_buffer_data	= reinterpret_cast<const float*>(&_buf_t.data[_view_t.byteOffset + _acc_t.byteOffset]);
				int			 time_buffer_stride	= _acc_t.ByteStride(_view_t);
				for (size_t i = 0; i < _acc_t.count; i++)
				{
					KeyFrame _frame;
					_frame.time = time_buffer_data[i * time_buffer_stride];
					_chain.addKeyFrame(_frame);
				}
				_anim->addKeyChain(_chain);
			}
			_engine->addAnimation(_anim);
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
				Matrix4 _mat;
				if (_skin.inverseBindMatrices != -1)
				{
					const tinygltf::Accessor& _acc = m_gltf_model->accessors[_skin.inverseBindMatrices];
					//TODO
				}
				int _idx = 0;
				for (int j : _skin.joints)
				{
					const tinygltf::Node& _node_joint = m_gltf_model->nodes[j];

					myEngine::Node* _node = new myEngine::Node();
					if (!_node_joint.scale.empty())
					{
						_node->setScale(Vector3(
							static_cast<float>(_node_joint.scale[0]),
							static_cast<float>(_node_joint.scale[1]),
							static_cast<float>(_node_joint.scale[2])));
					}
					if (!_node_joint.rotation.empty())
					{
						_node->setRotate(Quaternion(
							static_cast<float>(_node_joint.rotation[0]),
							static_cast<float>(_node_joint.rotation[1]),
							static_cast<float>(_node_joint.rotation[2]),
							static_cast<float>(_node_joint.rotation[3])));
					}
					if (!_node_joint.translation.empty())
					{
						_node->setTranslate(Vector3(
							static_cast<float>(_node_joint.translation[0]),
							static_cast<float>(_node_joint.translation[1]),
							static_cast<float>(_node_joint.translation[2])));
					}
					m_joint_nodes.push_back(_node);
					m_joint_idxs.insert(std::make_pair(j, _idx));
					_idx++;

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
		return static_cast<int>(m_joint_nodes.size());
	}

	const Matrix4* Model::getJointMatrixsData() const
	{
		std::vector<Matrix4> m_joint_matrixs;
		m_joint_matrixs.clear();
		m_joint_matrixs.reserve(m_joint_nodes.size());

		for (const Node* _n : m_joint_nodes)
		{
			Matrix4 _sm;
			_sm.initWithScale(_n->getScale());
			Matrix4 _rm;
			_rm.initWithRotateQuaternion(_n->getRotate());
			Matrix4 _tm;
			_tm.initWithTranslate(_n->getTranslate());

			Matrix4 _joint_matrix = _sm * _rm * _tm;
			m_joint_matrixs.push_back(_joint_matrix);
		}
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
