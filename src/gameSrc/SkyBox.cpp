//#include "SkyBox.h"
//
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif // STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//#include "Camera.h"
//#include "Mesh.h"
//
//#include "config.h"
//
//using namespace std;
//
//float cubeVerticeNum = 36;
//float cubeVertices[] = {
//	// Positions          // Texture Coords
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//};
//float skyboxVertices[] = {
//	// Positions          
//	-1.0f,  1.0f, -1.0f,
//	-1.0f, -1.0f, -1.0f,
//	1.0f, -1.0f, -1.0f,
//	1.0f, -1.0f, -1.0f,
//	1.0f,  1.0f, -1.0f,
//	-1.0f,  1.0f, -1.0f,
//
//	-1.0f, -1.0f,  1.0f,
//	-1.0f, -1.0f, -1.0f,
//	-1.0f,  1.0f, -1.0f,
//	-1.0f,  1.0f, -1.0f,
//	-1.0f,  1.0f,  1.0f,
//	-1.0f, -1.0f,  1.0f,
//
//	1.0f, -1.0f, -1.0f,
//	1.0f, -1.0f,  1.0f,
//	1.0f,  1.0f,  1.0f,
//	1.0f,  1.0f,  1.0f,
//	1.0f,  1.0f, -1.0f,
//	1.0f, -1.0f, -1.0f,
//
//	-1.0f, -1.0f,  1.0f,
//	-1.0f,  1.0f,  1.0f,
//	1.0f,  1.0f,  1.0f,
//	1.0f,  1.0f,  1.0f,
//	1.0f, -1.0f,  1.0f,
//	-1.0f, -1.0f,  1.0f,
//
//	-1.0f,  1.0f, -1.0f,
//	1.0f,  1.0f, -1.0f,
//	1.0f,  1.0f,  1.0f,
//	1.0f,  1.0f,  1.0f,
//	-1.0f,  1.0f,  1.0f,
//	-1.0f,  1.0f, -1.0f,
//
//	-1.0f, -1.0f, -1.0f,
//	-1.0f, -1.0f,  1.0f,
//	1.0f, -1.0f, -1.0f,
//	1.0f, -1.0f, -1.0f,
//	-1.0f, -1.0f,  1.0f,
//	1.0f, -1.0f,  1.0f
//};
//
//// Loads a cubemap texture from 6 individual texture faces
//// Order should be:
//// +X (right)
//// -X (left)
//// +Y (top)
//// -Y (bottom)
//// +Z (front) 
//// -Z (back)
//GLuint loadCubemap(vector<const GLchar*> faces)
//{
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//	int width, height, nrComponents;
//	unsigned char* image;
//
//	for (GLuint i = 0; i < faces.size(); i++)
//	{
//		image = stbi_load(faces[i], &width, &height, &nrComponents, STBI_default);
//		GLenum format = GL_RGBA;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 2)
//			format = GL_RGB;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
//		stbi_image_free(image);
//	}
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//
//	return textureID;
//}
//
//
//SkyBox::SkyBox():
//m_mesh(NULL),
//m_scale(1.0f),
//m_theta(0.0f)
//{
//    m_position = Vector3(0.0f);
//}
//
//SkyBox::~SkyBox()
//{
//    if(m_mesh)
//    {
//        delete(m_mesh);
//		m_mesh = NULL;
//    }
//    if(m_shader)
//    {
//        delete(m_shader);
//        m_shader = NULL;
//    }
//}
//
//bool SkyBox::init(string fileName)
//{
//	vector<Vertex> vertices;
//	vector<unsigned int> indices;
//	vector<Texture> textures;
//
//	for (int i = 0; i < cubeVerticeNum; i++)
//	{
//		Vertex vertex;
//
//		float x = cubeVertices[i * 5 + 0];
//		float y = cubeVertices[i * 5 + 1];
//		float z = cubeVertices[i * 5 + 2];
//		vertex.Position = Vector3(x, y, z);
//		vertex.Normal = Vector3(0.0f, 0.0f, 0.0f);
//		float u = cubeVertices[i * 5 + 3];
//		float v = cubeVertices[i * 5 + 4];
//		vertex.TexCoords = Vector3(u, v, 0.f);
//		vertex.Tangent = Vector3(0.0f, 0.0f, 0.0f);
//		vertex.Bitangent = Vector3(0.0f, 0.0f, 0.0f);
//
//		vertices.push_back(vertex);
//
//		int indice
//		indices.push_back()
//	}
//    m_mesh = new Mesh(vertices, indices, textures);
//
//	for (int i = 0; i < cubeVerticeNum; i++)
//	{
//
//	}
//
//	string vs_path = Config::engine_res_path + "shader/" + "common.vs";
//	string fs_path = Config::engine_res_path + "shader/" + "common.fs";
//    m_shader = new Shader(vs_path.c_str(), fs_path.c_str());
//
//
//	// Cubemap (Skybox)
//	vector<const GLchar*> faces;
//	faces.push_back("skybox/right.jpg");
//	faces.push_back("skybox/left.jpg");
//	faces.push_back("skybox/top.jpg");
//	faces.push_back("skybox/bottom.jpg");
//	faces.push_back("skybox/back.jpg");
//	faces.push_back("skybox/front.jpg");
//	GLuint cubemapTexture = loadCubemap(faces);
//
//    return true;
//}
//
//
//void SkyBox::render()
//{
//    m_shader->use();
//    GameScene * gamescene = GameScene::getInstance();
//    if(!gamescene)
//        return;
//    Camera * camera = gamescene->getCamera();
//    if(!camera)
//        return;
//	Matrix4 projection = camera->GetProjectMatrix();
//    Matrix4 view = camera->GetViewMatrix();
//
//    Matrix4 scaleM;
//    scaleM.initWithScale(Vector3(m_scale));
//    Matrix4 rotateM;
//    rotateM.initWithRotate(Vector3(0.f, 1.f, 0.f), m_theta);
//    Matrix4 transM;
//    transM.initWithTranslate(m_position);
//
//    Matrix4 model = scaleM * rotateM * transM;
//
//    m_shader->setMat4("model", model);
//    m_shader->setMat4("view", view);
//    m_shader->setMat4("projection", projection);
//
//	glBindVertexArray(skyboxVAO);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glBindVertexArray(0);
//
//    m_shader->disuse();
//}