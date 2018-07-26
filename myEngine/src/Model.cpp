#include "Model.h"

#ifndef TINYGLTF_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#endif // TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION
extern "C"
{
#include "stb_image.h"
}

namespace myEngine
{
	unsigned int Model::TextureFromFile(const char * path, const string &directory, bool gamma)
	{
		string filename = string(path);
		filename = directory + "/" + filename;
		unsigned int textureID = 0;
	
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
	
	vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, int type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType(type)); i++)
		{
			aiString str;
			mat->GetTexture(aiTextureType(type), i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
			{
				if (strcmp(m_texturesLoaded[j].path.c_str(), str.C_Str()) == 0)
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
				texture.path = str.C_Str();
				textures.push_back(texture);
				m_texturesLoaded.push_back(texture);
			}
		}
		return textures;
	}
	
	void Model::loadModel(const string &path)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF gltf_ctx;
		std::string err;
		bool ret = false;
		std::cout << "Reading ASCII glTF" << std::endl;
		ret = gltf_ctx.LoadASCIIFromFile(&model, &err, path);
		if (!ret) {
			printf("Failed to parse glTF\n");
			if (!err.empty())
				printf("Err: %s\n", err.c_str());
		}
		m_directory = path.substr(0, path.find_last_of('/'));
	
		processNode(scene->mRootNode, scene);
	}
	
	void Model::processNode(aiNode * node, const aiScene * scene)
	{
		for(unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshs.push_back(processMesh(mesh, scene));
		}
	
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	
	Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
	
		for(unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			Vector3 vector;
	
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
	
			if (mesh->mNormals)
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}
			else
				vertex.Normal = Vector3(0.0f, 0.0f, 0.0f);
	
			if(mesh->mTextureCoords[0])
			{
				Vector3 vec = Vector3(0.0f);
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = Vector3(0.0f);
	
			// tangent
			if (mesh->mTangents)
			{
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
			}
			else
				vertex.Tangent = Vector3(0.0f, 0.0f, 0.0f);
			// bitangent
			if (mesh->mBitangents)
			{
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			else
				vertex.Bitangent = Vector3(0.0f, 0.0f, 0.0f);
	
			vertices.push_back(vertex);
	
		}
	
		for(unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
	
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
	
		if (material)
		{
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
			vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	
			vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
	
		return Mesh(vertices, indices, textures);
	}
}