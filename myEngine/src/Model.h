#ifndef __MODEL_H__
#define __MODEL_H__

#include "MyEngineCore.h"

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <iostream>
#include <vector>

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
		Model(const std::string &path);
		~Model();
	
		void draw();
	
	private:
		void loadModel(const std::string &path);

		void copyTexture();
	private:
		tinygltf::Model* m_gltf_model;
		std::vector<bgfx::TextureHandle> textrue_handles;
		std::vector<RenderObject*> m_render_objects;
	};
}

#endif //__MODEL_H__