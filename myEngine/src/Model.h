#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <iostream>
#include <vector>

#include "Matrix4.h"

#include "bgfx/bgfx.h"

namespace tinygltf
{
	class Model;
}

namespace myEngine
{
	class RenderObject;
	class Model
	{
	public:	
		Model();
		~Model();

		void load(const std::string &path);
	
		void draw();

		bgfx::TextureHandle getTextureHandle(int index);

		int getJointMatrixsNum() const;
		const Matrix4* getJointMatrixsData() const;

		float getScale() const;

		void setScale(float val);

		Matrix4 getModelMatrix();
	
	private:

		void _loadNodeTreeMesh(int node_id);

		void _createTextureHandle();

	private:
		tinygltf::Model* m_gltf_model;
		std::vector<bgfx::TextureHandle> m_textrue_handles;
		std::vector<RenderObject*> m_render_objects;
		std::vector<Matrix4> m_joint_matrixs;
		float  m_scale;
	};
}

#endif //__MODEL_H__