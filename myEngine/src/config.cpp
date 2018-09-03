#include "Config.h"
#include "tinyxml2.h"

namespace myEngine
{
	const string Config::INVALID_STR = "";

	const string Config::engine_res_path = "../../myEngine/res/";
	const string Config::shader_bin_path = engine_res_path + "shaderbin/";
	const string Config::game_res_path = "../../res/";
	const string Config::model_path = Config::game_res_path + "models/";
	const string Config::scene_path = Config::game_res_path + "scenes/";
	const string Config::lua_path = Config::game_res_path + "luafiles/";
	
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
}