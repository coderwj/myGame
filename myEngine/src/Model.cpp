#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Model.h"


unsigned int Model::TextureFromFile(const char * path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + filename;
	unsigned int textureID;

	int width, height, nrComponents;
	unsigned char * data = stbi_load(filename.c_str(), &width, &height, &nrComponents, STBI_default);
	if(data)
	{
		GLenum format = GL_RGBA;
		if(nrComponents == 1)
			format = GL_RED;
		else if(nrComponents == 2)
			format = GL_RGB;
		else if(nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		cout << "Texture failed to load, path: " << filename << endl;
		stbi_image_free(data);
	}

	return textureID;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
		{
			if (strcmp(m_texturesLoaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->m_directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			m_texturesLoaded.push_back(texture);
		}
	}
	return textures;
}
