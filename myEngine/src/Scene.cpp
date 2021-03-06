//#include "Scene.h"
//
//#include "Config.h"
//#include "tinyxml2.h"
//
//#include "SkyBox.h"
//#include "Camera.h"
//#include "Model.h"
//
//#include <string>
//
//using namespace std;
//
//namespace myEngine
//{
//	bool Scene::init() {
//
//		string sceneName = "scene_3";
//		loadScene(sceneName.c_str());
//
//		return true;
//	}
//	Scene::Scene() :
//		m_model(nullptr),
//		m_scale(1.0f),
//		m_theta(0.0f)
//	{
//		m_rotateVec = Vector3(0.0f, 1.0f, 0.0f);
//		m_fogpara.color = Vector3(1.0f, 1.0f, 1.0f);
//		m_fogpara.start = 10.f;
//		m_fogpara.end = 100.f;
//		m_fogpara.intensity = 1.f;
//	}
//
//	Scene::~Scene()
//	{
//
//	}
//
//	void Scene::onDestroy()
//	{
//	}
//
//	void Scene::render()
//	{
//		_renderScene();
//	}
//
//	void Scene::_renderScene()
//	{
//		m_model->Draw();
//	}
//
//	void Scene::tick(float delta)
//	{
//
//	}
//
//	void Scene::loadScene(const char* sceneName)
//	{
//		string model_str = "";
//		string material_name = "";
//		string scene_config_path = Config::scene_path + "sceneConfig.xml";
//		tinyxml2::XMLDocument scene_model_doc;
//		scene_model_doc.LoadFile(scene_config_path.c_str());
//		tinyxml2::XMLElement* scene_model_element =
//			scene_model_doc.FirstChildElement("sceneconfig")->FirstChildElement("scene");
//		for (;; scene_model_element = scene_model_element->NextSiblingElement("scene")) {
//			if (scene_model_element == NULL)
//				break;
//			if (sceneName == scene_model_element->FirstChildElement("name")->GetText())
//			{
//				model_str = scene_model_element->FirstChildElement("path")->GetText();
//				material_name = scene_model_element->FirstChildElement("material")->GetText();
//				m_scale = scene_model_element->FirstChildElement("scale")->FloatText(1.0f);
//				m_theta = scene_model_element->FirstChildElement("rotateTheta")->FloatText(0.0f);
//				float rotateX = scene_model_element->FirstChildElement("rotateX")->FloatText(0.0f);
//				float rotateY = scene_model_element->FirstChildElement("rotateY")->FloatText(1.0f);
//				float rotateZ = scene_model_element->FirstChildElement("rotateZ")->FloatText(0.0f);
//				m_rotateVec = Vector3(rotateX, rotateY, rotateZ);
//
//				tinyxml2::XMLElement* fog_element = scene_model_element->FirstChildElement("fogpara");
//				if (NULL != fog_element)
//				{
//					float red = fog_element->FirstChildElement("color")->FirstChildElement("red")->FloatText(m_fogpara.color.x);
//					float green = fog_element->FirstChildElement("color")->FirstChildElement("green")->FloatText(m_fogpara.color.y);
//					float blue = fog_element->FirstChildElement("color")->FirstChildElement("blue")->FloatText(m_fogpara.color.z);
//					m_fogpara.color = Vector3(red, green, blue);
//					m_fogpara.start = fog_element->FirstChildElement("start")->FloatText(m_fogpara.start);
//					m_fogpara.end = fog_element->FirstChildElement("end")->FloatText(m_fogpara.end);
//					m_fogpara.intensity = fog_element->FirstChildElement("intensity")->FloatText(m_fogpara.intensity);
//				}
//				break;
//			}
//		}
//		if (model_str == "")
//		{
//			return;
//		}
//		m_model = new Model(Config::scene_path + model_str);
//	}
//
//	void Scene::changeScene(const char* sceneName)
//	{
//		if (m_model)
//		{
//			delete m_model;
//			m_model = NULL;
//		}
//		loadScene(sceneName);
//	}
//
//}
