#include "SkyBox.h"

//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif // STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.h"
#include "Shader.h"

#include <string>

using namespace std;

namespace myEngine
{
	const int skyboxVerticeNum = 36;
	const float skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
	
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
	
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
	
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
	
		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
	
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
	
	unsigned int loadCubemap(vector<string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
		int width, height, nrComponents;
		unsigned char* image;
	
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			image = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, STBI_default);
			GLenum format = GL_RGBA;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 2)
				format = GL_RGB;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;
	
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
		return textureID;
	}
	
	
	SkyBox::SkyBox():
	m_scale(100.0f),
	m_theta(0.0f)
	{
	    m_position = Vector3(0.0f);
	}
	
	SkyBox::~SkyBox()
	{
	    if(m_shader)
	    {
	        delete(m_shader);
	        m_shader = NULL;
	    }
	}
	
	bool SkyBox::init(const char*  fileName)
	{
	
		// Cubemap (Skybox)
		string skybox_path = Config::game_res_path + "skybox/";
		string skybox_name = Config::GetConfigStr("skybox_name");
		string skybox_type = Config::GetConfigStr("skybox_type");
		vector<string> faces;
		faces.push_back(skybox_path + skybox_name + "r" + skybox_type);
		faces.push_back(skybox_path + skybox_name + "l" + skybox_type);
		faces.push_back(skybox_path + skybox_name + "u" + skybox_type);
		faces.push_back(skybox_path + skybox_name + "d" + skybox_type);
		faces.push_back(skybox_path + skybox_name + "b" + skybox_type);
		faces.push_back(skybox_path + skybox_name + "f" + skybox_type);
		cubemapTexture = loadCubemap(faces);
	
		string vs_path = Config::engine_res_path + "shader/" + "skybox.vs";
		string fs_path = Config::engine_res_path + "shader/" + "skybox.fs";
		m_shader = new Shader(vs_path.c_str(), fs_path.c_str());
	
		m_scale = static_cast<float>(::atoi(Config::GetConfigStr("skybox_scale").c_str()));
	
	
		vector<float> vertices;
	
		for (int i = 0; i < skyboxVerticeNum; i++)
		{
			float x = skyboxVertices[i * 3 + 0];
			float y = skyboxVertices[i * 3 + 1];
			float z = skyboxVertices[i * 3 + 2];
			vertices.push_back(x * m_scale);
			vertices.push_back(y * m_scale);
			vertices.push_back(z * m_scale);
			float u = skyboxVertices[i * 3 + 0];
			float v = skyboxVertices[i * 3 + 1];
			float w = skyboxVertices[i * 3 + 2];
			vertices.push_back(u);
			vertices.push_back(v);
			vertices.push_back(w);
		}
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), static_cast<const void*>(&vertices[0]), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	    return true;
	}
	
	
	void SkyBox::render()
	{
	    m_shader->use();
	    GameScene * gamescene = GameScene::getInstance();
	    if(!gamescene)
	        return;
	    Camera * camera = gamescene->getCamera();
	    if(!camera)
	        return;
		Matrix4 projection = camera->GetProjectMatrix();
	    Matrix4 view = camera->GetViewMatrix();
	
	    Matrix4 scaleM;
	    scaleM.initWithScale(Vector3(1.f));
	    Matrix4 rotateM;
	    rotateM.initWithRotate(Vector3(0.f, 1.f, 0.f), m_theta);
	    Matrix4 transM;
	    transM.initWithTranslate(m_position);
	
	    Matrix4 model = scaleM * rotateM * transM;
	
	    m_shader->setMat4("model", model);
	    m_shader->setMat4("view", view);
	    m_shader->setMat4("projection", projection);
	
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	
	    m_shader->disuse();
	}
}