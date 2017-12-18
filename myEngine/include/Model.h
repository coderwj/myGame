#ifndef __MODEL_H__
#define __MODEL_H__

#include "MyEngineCore.h"

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

class Model
{
public:
	vector<Texture> m_texturesLoaded;
	vector<Mesh> m_meshs;
	string m_directory;
	bool m_gammaCorrection;

	Model(const string &path, bool gamma = false):
	m_gammaCorrection(gamma)
	{
		loadModel(path);
	}

	void Draw(Shader &shader)
	{
		for (unsigned int i = 0; i < m_meshs.size(); i++)
			m_meshs[i].Draw(shader);
	}

private:
	void loadModel(const string &path);

	void processNode(aiNode * node, const aiScene * scene);

	Mesh processMesh(aiMesh * mesh, const aiScene * scene);

	vector<Texture> loadMaterialTextures(aiMaterial * mat, int type, string typeName);

	unsigned int TextureFromFile(const char * path, const string &directory, bool gamma = false);
};

#endif //__MODEL_H__