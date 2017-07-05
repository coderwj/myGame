
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

using namespace std;

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

	void Draw(Shader shader)
	{
		for(unsigned int i = 0; i < m_meshs; i++)
			m_meshs[i].Draw(shader)
	}

private:
	void loadModel(const string &path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace)
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		m_directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode * node, const aiScene * scene)
	{
		for(unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshs.push_back(processMesh(mesh, scene));
		}
	}

	Mesh processMesh(aiMesh * mesh, const aiScene * scene)
	{
		
	}
}