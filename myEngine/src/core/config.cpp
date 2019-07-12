#include "config.h"
#include "tinyxml2.h"
#include <assert.h>

namespace myEngine
{
    const string Config::INVALID_STR = "";
#ifdef _EDITOR
    string Config::engine_res_path = "../../../myEngine/res/";
    string Config::game_res_path = "../../../res/";
#elif __APPLE__
    string Config::engine_res_path = "";
    string Config::game_res_path = "";
#else
    string Config::engine_res_path = "../../../../../myEngine/res/";
    string Config::game_res_path = "../../../../../res/";
#endif // DEBUG
    string Config::shader_bin_path = Config::engine_res_path + "shaderbin/";
    string Config::shader_src_path = Config::engine_res_path + "shader/";
    string Config::model_path = Config::game_res_path + "models/";
    string Config::scene_path = Config::game_res_path + "scenes/";
    string Config::lua_path = Config::game_res_path + "luafiles/";
    
    const unordered_map<string, string> Config::configMap = unordered_map<string, string>();
    
    
    void Config::InitConfigMap()
    {
        unordered_map<string, string>* temp = const_cast<unordered_map<string, string>*>(&configMap);
        string key = "";
        string value = "";
        string conifg_path = Config::game_res_path + "commonConfig.xml";
        tinyxml2::XMLDocument config_doc;
        tinyxml2::XMLError res = config_doc.LoadFile(conifg_path.c_str());
        if (tinyxml2::XML_SUCCESS != res)
        {
            assert(false);
        }
        tinyxml2::XMLElement* config_element =
            config_doc.FirstChildElement("commonconfig")->FirstChildElement();
        for (;; config_element = config_element->NextSiblingElement()) {
            if (config_element == NULL)
                break;
            key = config_element->Name();
            value = config_element->Attribute("v");
            temp->insert(pair<string, string>(key, value));
        }
    }
    
    bool Config::GetIsShaderTest()
    {
        if (configMap.find("shader_test") != configMap.end())
        {
            string resStr = configMap.at("shader_test");
            return resStr == "true";
        }
        return false;
    }
    
    const string& Config::GetConfigStr(const string& key)
    {
        if (configMap.find(key) != configMap.end())
        {
            const string& resStr = configMap.at(key);
            return resStr;
        }
        return INVALID_STR;
    }

    void Config::getShaderPathByMaterialName(std::string material, std::string& vs_path, std::string& fs_path, std::string& varying_path)
    {
        std::string shader_config_path = Config::engine_res_path + "materialConfig.xml";
        tinyxml2::XMLDocument shader_doc;
        shader_doc.LoadFile(shader_config_path.c_str());

        tinyxml2::XMLElement* materialElement = shader_doc.FirstChildElement("materialconfig")->FirstChildElement("material");
        for (; materialElement != NULL; materialElement = materialElement->NextSiblingElement("material"))
        {
            if (material == materialElement->FirstChildElement("name")->GetText())
            {
                vs_path = Config::shader_src_path + materialElement->FirstChildElement("vertexshader")->GetText();
                fs_path = Config::shader_src_path + materialElement->FirstChildElement("fragmentshader")->GetText();
                varying_path = Config::shader_src_path + materialElement->FirstChildElement("varyingdef")->GetText();
                return;
            }
        }
        assert(false);
    }
}
