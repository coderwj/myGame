#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "Matrix4.h"
#include "Vector3.h"

#include "bgfx/bgfx.h"

namespace tinygltf
{
	class Model;
}

namespace myEngine
{
	class Node;
	class RenderObject;
	class Animation;

	class Skeleton
	{
	public:
		Skeleton()
		:m_root_idx(0)
		{

		}
		~Skeleton() { }
		int m_root_idx;
		std::vector<int> m_joint_idxs;
		std::vector<Matrix4> m_joint_inverse_mats;
		static int MAX_JOINT_NUM;
	};

	class Model
	{
	public:	
		Model();
		~Model();

		void load(const std::string &path);
		void tick(int delta);
		void draw();

		bgfx::TextureHandle getTextureHandle(int index);

		const Matrix4* getJointMatrixsData();

		float getScale() const;

		void setScale(float val);

		Matrix4 getModelMatrix();
	
	private:

		void _loadTextures();

		void _loadNodes();
		void _loadNodeRecursive(int node_id, Node* parent);

		void _loadAnimations();

		void _updateNodeTransformToChilren(Node* parent);

	private:
		tinygltf::Model* m_gltf_model;
		std::vector<bgfx::TextureHandle> m_textrue_handles;
		std::vector<RenderObject*> m_render_objects;

		std::vector<Animation*> m_animations;

		std::vector<Matrix4> m_joint_matrixs;

		std::vector<int>	m_roots;
		std::vector<Node*> m_nodes;
		std::map<int, Node*> m_node_map; //k: gltf file node idx, v: Node* in m_nodes vector.
		float  m_scale;

		Skeleton* m_skeleton;
	};
}

#endif //__MODEL_H__