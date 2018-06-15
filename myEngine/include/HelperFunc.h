#ifndef __HELPER_FUNC_H__
#define __HELPER_FUNC_H__

class HelperFunc
{
public:
	static bool convToWinPath(char * path);

	static size_t getFileSize(const char *fileName);

	static bool LoadFromFile(const char* const fileName, char* const buff, unsigned int buff_size);

	static bool WriteToFile(const char* const fileName, char* const buff);

};

#endif //__HELPER_FUNC_H__