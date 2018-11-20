#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

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
		:m_root_idx(-1)
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

		const Matrix4* getJointMatrixsData(Node* mesh_node);

		const Matrix4& getModelMatrix();
		const Matrix4& getWorldMatrix();

		void setModelMatrix(const Matrix4& w);
		void setWorldMatrix(const Matrix4& w);

		bool getVisible() const { return m_visible; }
		void setVisible(bool val) { m_visible = val; }
	
	private:

		void _loadTextures();

		void _loadNodes();
		void _loadNodeRecursive(int node_id, Node* parent);

		void _loadAnimations();

		void _updateNodeTransformToChilren(Node* parent);

	private:
		//tinygltf model
		tinygltf::Model* m_gltf_model;

		//texture handles
		std::vector<bgfx::TextureHandle> m_textrue_handles;

		//render objects
		std::vector<RenderObject*> m_render_objects;

		//animations
		std::vector<Animation*> m_animations;

		//joint matrix
		std::vector<Matrix4> m_joint_matrixs;

		//node tree
		std::vector<int> m_roots;
		std::vector<Node*> m_nodes;
		std::unordered_map<int, Node*> m_node_map; //k: gltf file node idx, v: Node* in m_nodes vector.
		
		//model matrix
		Matrix4 m_model_matrix;

		//world matrix
		Matrix4 m_world_matrix;

		//skeleton
		Skeleton* m_skeleton;

		bool m_visible;
	};
}

#endif //__MODEL_H__