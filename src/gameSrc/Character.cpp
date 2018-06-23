#include "Character.h"

#include "GameScene.h"
#include "Camera.h"
#include "Model.h"

#include "config.h"

#include "tinyxml2.h"

using namespace std;

Character::Character():
m_modelName(""),
m_model(NULL),
m_scale(1.0f),
m_theta(0.0f)
{
    m_position = Vector3(0.0f);
    m_orientation = Vector3(1.0f, 0.0f, .0f);
    m_rotateVec = Vector3(0.0f, 1.0f, 0.0f);
}

Character::~Character()
{
    if(m_model)
    {
        delete(m_model);
        m_model = NULL;
    }
    if(m_shader)
    {
        delete(m_shader);
        m_shader = NULL;
    }
}

bool Character::init(string modelName, Vector3 position, Vector3 orientation)
{
    string model_str = "";
    string material_name = "";
    string model_config_path = Config::model_path + "modelConfig.xml";
    tinyxml2::XMLDocument character_model_doc;
    character_model_doc.LoadFile(model_config_path.c_str());
    tinyxml2::XMLElement* character_model_element =
    character_model_doc.FirstChildElement("modelconfig")->FirstChildElement("model");
    for (;;character_model_element = character_model_element->NextSiblingElement("model")) {
        if(character_model_element == NULL)
            break;
        if(modelName == character_model_element->FirstChildElement("name")->GetText())
        {
            model_str = character_model_element->FirstChildElement("path")->GetText();
            material_name = character_model_element->FirstChildElement("material")->GetText();
            m_scale = character_model_element->FirstChildElement("scale")->FloatText(1.0f);
            m_theta = character_model_element->FirstChildElement("rotateTheta")->FloatText(0.0f);
            float rotateX = character_model_element->FirstChildElement("rotateX")->FloatText(0.0f);
            float rotateY = character_model_element->FirstChildElement("rotateY")->FloatText(1.0f);
            float rotateZ = character_model_element->FirstChildElement("rotateZ")->FloatText(0.0f);
            m_rotateVec = Vector3(rotateX, rotateY, rotateZ);
            break;
        }
    }
    if(model_str == "")
    {
        //todo log info
        return false;
    }
    m_model = new Model(Config::model_path + model_str);

    string shader_config_path = Config::engine_res_path + "shader/shaderConfig.xml";
    tinyxml2::XMLDocument shader_doc;
    shader_doc.LoadFile(shader_config_path.c_str());

    string vs_name = "", fs_name = "";

    tinyxml2::XMLElement* materialElement = shader_doc.FirstChildElement("shaderconfig")->FirstChildElement( "material" );
    for (;;materialElement = materialElement->NextSiblingElement("material")) {
        if(materialElement == NULL)
        {
            break;
        }
        if(material_name == materialElement->FirstChildElement("name")->GetText())
        {
            vs_name = materialElement->FirstChildElement("vertexshader")->GetText();
            fs_name = materialElement->FirstChildElement("fragmentshader")->GetText();
            break;
        }
    }
    if(vs_name == "" || fs_name == "")
    {
        return false;
        //assert(true);
    }
    string vs_path = Config::engine_res_path + "shader/" + vs_name;
    string fs_path = Config::engine_res_path + "shader/" + fs_name;

    m_shader = new Shader(vs_path.c_str(), fs_path.c_str());

    return true;
}

Character * Character::Create(string modelName, Vector3 position, Vector3 orientation)
{
    Character * cha = new Character();
    if(cha->init(modelName, position, orientation))
        return cha;
    else
        return NULL;
}

void Character::render()
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
    scaleM.initWithScale(Vector3(m_scale));
    Matrix4 rotateM;
    rotateM.initWithRotate(m_rotateVec, m_theta);
    Matrix4 transM;
    transM.initWithTranslate(m_position);

    Matrix4 model = scaleM * rotateM * transM;

    m_shader->setMat4("model", model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    m_shader->setVec3("light_color", Vector3(1.0f, 1.0f, 1.0f));
    m_shader->setVec3("light_dir", Vector3(1.0f, 1.0f, 1.0f));
    m_shader->setVec3("Ka", Vector3(0.2f, 0.2f, 0.2f));
    m_shader->setVec3("Kd", Vector3(1.0f, 1.0f, 1.0f));
    m_shader->setVec3("Ks", Vector3(1.0f, 1.0f, 1.0f));
    m_shader->setVec3("view_pos", camera->Position);

    m_model->Draw(*m_shader);
    m_shader->disuse();
}