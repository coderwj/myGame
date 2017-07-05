
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
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
	}
}