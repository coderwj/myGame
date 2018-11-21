#include "Model.h"

#include <assert.h>

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

	int Skeleton::MAX_JOINT_NUM = 64;

	Model::Model()
	:m_gltf_model(nullptr)
	,m_skeleton(nullptr)
	,m_visible(true)
	{
	}

	Model::~Model()
	{
		if (m_gltf_model)
		{
			delete m_gltf_model;
			m_gltf_model = nullptr;
		}

		if (m_skeleton)
		{
			delete m_skeleton;
			m_skeleton = nullptr;
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

		_loadTextures();

		_loadNodes();

		_loadAnimations();
	}

	void Model::_loadTextures()
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

			unsigned long long _flags = 0;
			//_flags |= BGFX_TEXTURE_SRGB;

			bgfx::TextureHandle _th = bgfx::createTexture2D(static_cast<uint16_t>(_img.width),
															static_cast<uint16_t>(_img.height),
															false,
															1,
															_format,
															_flags,
															_data);
			_img.image.clear();
			_img.image.reserve(0);
			m_textrue_handles.push_back(_th);
		}
	}

	void Model::_loadNodes()
	{
		int _id = m_gltf_model->defaultScene;
		const tinygltf::Scene& _scene = m_gltf_model->scenes[_id];
		for (int n : _scene.nodes)
		{
			_loadNodeRecursive(n, nullptr);
		}
	}

	void Model::_loadNodeRecursive(int node_id, Node* parent)
	{
		const tinygltf::Node& _node = m_gltf_model->nodes[node_id];
		myEngine::Node* _my_node = new myEngine::Node();

		//parent
		if (nullptr != parent)
		{
			parent->addChild(_my_node);
			_my_node->setParent(parent);
		}

		//node transform
		if (!_node.matrix.empty())
		{
			Matrix4 m(_node.matrix.data());
			Vector3 s;
			Quaternion r;
			Vector3 t;
			m.deCompose(s, r, t);
			_my_node->setScale(s);
			_my_node->setRotate(r.normalize());
			_my_node->setTranslate(t);
		}
		else
		{
			if (!_node.scale.empty())
			{
				_my_node->setScale(Vector3(
					static_cast<float>(_node.scale[0]),
					static_cast<float>(_node.scale[1]),
					static_cast<float>(_node.scale[2])));
			}
			if (!_node.rotation.empty())
			{
				_my_node->setRotate(Quaternion(
					static_cast<float>(_node.rotation[0]),
					static_cast<float>(_node.rotation[1]),
					static_cast<float>(_node.rotation[2]),
					static_cast<float>(_node.rotation[3])).normalize());
			}
			if (!_node.translation.empty())
			{
				_my_node->setTranslate(Vector3(
					static_cast<float>(_node.translation[0]),
					static_cast<float>(_node.translation[1]),
					static_cast<float>(_node.translation[2])));
			}
		}

		//mesh node
		if (_node.mesh != -1)
		{
			const tinygltf::Mesh& _mesh = m_gltf_model->meshes[_node.mesh];
			for (const tinygltf::Primitive& p : _mesh.primitives)
			{
				RenderObject* _render_object = new RenderObject();
				_render_object->init(this, _my_node, p, *m_gltf_model);
				m_render_objects.push_back(_render_object);
			}

			//skin
			if (_node.skin != -1)
			{
				const tinygltf::Skin& _skin = m_gltf_model->skins[_node.skin];
				assert(nullptr == m_skeleton);
				m_skeleton = new Skeleton();
				m_skeleton->m_root_idx = _skin.skeleton;
				m_skeleton->m_joint_idxs = _skin.joints;

				if (_skin.inverseBindMatrices != -1)
				{
					const tinygltf::Accessor&	_acc	= m_gltf_model->accessors[_skin.inverseBindMatrices];
					const tinygltf::BufferView& _view	= m_gltf_model->bufferViews[_acc.bufferView];
					const tinygltf::Buffer&		_buf	= m_gltf_model->buffers[_view.buffer];
					const float* _data = reinterpret_cast<const float*>(&_buf.data[_view.byteOffset + _acc.byteOffset]);
					m_skeleton->m_joint_inverse_mats.reserve(m_skeleton->m_joint_idxs.size());
					for (size_t i = 0; i < m_skeleton->m_joint_idxs.size(); i++)
					{
						Matrix4 _mat(_data);
						m_skeleton->m_joint_inverse_mats.push_back(_mat);
						_data += 16;
					}
				}
				else
				{
					for (size_t i = 0; i < m_skeleton->m_joint_idxs.size(); i++)
					{
						Matrix4 _mat;
						m_skeleton->m_joint_inverse_mats.push_back(_mat);
					}
				}
			}
		}

		m_node_map.insert(std::make_pair(node_id, _my_node));
		m_nodes.push_back(_my_node);

		//load children recursive
		if (!_node.children.empty())
		{
			for (int i : _node.children)
				_loadNodeRecursive(i, _my_node);
		}
	}

	void Model::_loadAnimations()
	{
		m_animations.reserve(m_gltf_model->animations.size());
		for (const tinygltf::Animation& a : m_gltf_model->animations)
		{
			Animation *_anim = new Animation();
			m_animations.push_back(_anim);
			for (const tinygltf::AnimationChannel& c : a.channels)
			{
				int node_index = c.target_node;
				KeyChain _chain;
				const std::string& path = c.target_path;
				//"translation", "rotation", "scale", "weights"
				if (path == "translation")
				{
					_chain.setType(KEY_CHAIN_TYPE_V3);
					_chain.setTargetType(KEY_CHAIN_TARGET_TRANSLATE);
				}
				else if (path == "rotation")
				{
					_chain.setType(KEY_CHAIN_TYPE_V4);
					_chain.setTargetType(KEY_CHAIN_TARGET_ROTATE);
				}
				else if (path == "scale")
				{
					_chain.setType(KEY_CHAIN_TYPE_V3);
					_chain.setTargetType(KEY_CHAIN_TARGET_SCALE);
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
				const tinygltf::Accessor&	_acc_t = m_gltf_model->accessors[s.input];
				const tinygltf::BufferView& _view_t = m_gltf_model->bufferViews[_acc_t.bufferView];
				const tinygltf::Buffer&		_buf_t = m_gltf_model->buffers[_view_t.buffer];
				const float* time_buffer_data = reinterpret_cast<const float*>(&_buf_t.data[_view_t.byteOffset + _acc_t.byteOffset]);

				//value accessor
				const tinygltf::Accessor&	_acc_v = m_gltf_model->accessors[s.output];
				const tinygltf::BufferView& _view_v = m_gltf_model->bufferViews[_acc_v.bufferView];
				const tinygltf::Buffer&		_buf_v = m_gltf_model->buffers[_view_v.buffer];
				const float* value_buffer_data = reinterpret_cast<const float*>(&_buf_v.data[_view_v.byteOffset + _acc_v.byteOffset]);

				//TODO: Animation time and value current are only float data. not support other types.

				for (size_t i = 0; i < _acc_t.count; i++)
				{
					KeyFrame _frame;
					_frame.time = time_buffer_data[i];
					size_t _size = _chain.getType();
					_frame.values.reserve(_size);
					for (size_t j = 0; j < _size; j++)
					{
						_frame.values.push_back(value_buffer_data[i * _size + j]);
					}
					_chain.addKeyFrame(_frame);
				}
				_chain.setTarget(m_node_map[node_index]);
				_chain.sortKeyFrames();
				_anim->addKeyChain(_chain);
			}
		}
	}

	bgfx::TextureHandle Model::getTextureHandle(int index)
	{
		if (index < 0)
			return BGFX_INVALID_HANDLE;
		if (index >= static_cast<int>(m_textrue_handles.size()))
			return BGFX_INVALID_HANDLE;
		return m_textrue_handles[index];
	}

	const Matrix4* Model::getJointMatrixsData(Node* mesh_node)
	{
		m_joint_matrixs.clear();
		m_joint_matrixs.reserve(Skeleton::MAX_JOINT_NUM);

		if (nullptr == m_skeleton)
		{
			return m_joint_matrixs.data();
		}

		if (nullptr == mesh_node)
		{
			return m_joint_matrixs.data();
		}

		Matrix4 inverseGlobalMatrix;
		inverseGlobalMatrix = mesh_node->generateGlobalMatrix();
		inverseGlobalMatrix.inverse();

		for (size_t i = 0; i < m_skeleton->m_joint_idxs.size(); i++)
		{
			int idx = m_skeleton->m_joint_idxs[i];
			Node* _n = m_node_map[idx];

			Matrix4 _joint_matrix = _n->generateGlobalMatrix();

			m_joint_matrixs.push_back(m_skeleton->m_joint_inverse_mats[i] * _joint_matrix * inverseGlobalMatrix);
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

	const Matrix4& Model::getModelMatrix()
	{
		return m_model_matrix;
	}

	void Model::setModelMatrix(const Matrix4 & w)
	{
		m_model_matrix = w;
	}

	const Matrix4& Model::getWorldMatrix()
	{
		return m_world_matrix;
	}

	void Model::setWorldMatrix(const Matrix4 & w)
	{
		m_world_matrix = w;
	}

	void Model::tick(int delta)
	{
		for (Animation * _anim : m_animations)
		{
			_anim->tick(delta);
		}
		if (m_visible)
		{
			draw();
		}
	}

}
