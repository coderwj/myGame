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
	
		void Draw(Shader &shader);
	
	private:
		void loadModel(const std::string &path);

		void copyTexture();
	
		unsigned int TextureFromFile(const char * path, const std::string &directory, bool gamma = false);
	private:
		tinygltf::Model* m_gltf_model;
		std::vector<bgfx::TextureHandle> textrue_handles;
		RenderObject* m_render_object;
	};
}

#endif //__MODEL_H__