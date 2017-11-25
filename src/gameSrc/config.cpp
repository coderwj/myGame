#include "Config.h"
#include "tinyxml2.h"

#ifdef WIN32
    const string Config::engine_res_path = "../../../../../myEngine/res/";
    const string Config::game_res_path = "../../../../../res/";
    const string Config::model_path = Config::game_res_path + "models/";
    const string Config::scene_path = Config::game_res_path + "scenes/";
    const string Config::lua_path = Config::game_res_path + "luafiles/";
#else
    const string Config::engine_res_path = "../../../../../../myEngine/res/";
    const string Config::game_res_path = "../../../../../../res/";
    const string Config::model_path = Config::game_res_path + "models/";
    const string Config::scene_path = Config::game_res_path + "scenes/";
    const string Config::lua_path = Config::game_res_path + "luafiles/";
#endif

const unordered_map<string, string> Config::configMap = unordered_map<string, string>();


void Config::InitConfigMap()
{
    unordered_map<string, string>* temp = const_cast<unordered_map<string, string>*>(&configMap);
	string key = "";
	string value = "";
	string conifg_path = Config::game_res_path + "commonConfig.xml";
	tinyxml2::XMLDocument config_doc;
	config_doc.LoadFile(conifg_path.c_str());
	tinyxml2::XMLElement* config_element =
		config_doc.FirstChildElement("commonconfig")->FirstChildElement("config");
	for (;; config_element = config_element->NextSiblingElement("config")) {
		if (config_element == NULL)
			break;
		key = config_element->FirstChildElement("key")->GetText();
		value = config_element->FirstChildElement("value")->GetText();
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