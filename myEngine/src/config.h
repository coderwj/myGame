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
		static const string INVALID_STR;
		static const string engine_res_path;
		static const string shader_bin_path;
		static const string shader_src_path;
		static const string game_res_path;
		static const string model_path;
		static const string scene_path;
		static const string lua_path;
	
	    static const unordered_map<string, string> configMap;
	
	    static void InitConfigMap();
	
		static bool GetIsShaderTest();
	
		static const string& GetConfigStr(const string& key);
	};
}

#endif //__CONFIG_H__