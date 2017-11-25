#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <unordered_map>

using namespace std;

class Config
{
public:
	static const string engine_res_path;
	static const string game_res_path;
	static const string model_path;
	static const string scene_path;
	static const string lua_path;

    static const unordered_map<string, string> configMap;

    static void InitConfigMap();

	static bool GetIsShaderTest();
};

#endif //__CONFIG_H__