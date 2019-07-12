#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <unordered_map>

using namespace std;

namespace myEngine
{
	class Config
	{
	public:
		const static string INVALID_STR;
		static string engine_res_path;
		static string shader_bin_path;
		static string shader_src_path;
		static string game_res_path;
		static string model_path;
		static string scene_path;
		static string lua_path;
	
	    static const unordered_map<string, string> configMap;
	
	    static void InitConfigMap();
	
		static bool GetIsShaderTest();
	
		static const string& GetConfigStr(const string& key);

		static void getShaderPathByMaterialName(std::string material, std::string& vs_path, std::string& fs_path, std::string& varying_path);
	};
}

#endif //__CONFIG_H__
