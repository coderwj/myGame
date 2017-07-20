#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

using namespace std;

class Config
{
public:
	static const string engine_res_path;
	static const string game_res_path;
	static const string model_path;
	static const string scene_path;
};

#endif //__CONFIG_H__