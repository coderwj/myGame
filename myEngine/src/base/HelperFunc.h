#ifndef __HELPER_FUNC_H__
#define __HELPER_FUNC_H__

namespace myEngine
{
	class HelperFunc
	{
	public:
		static bool convToWinPath(char * path);
	
		static int getFileSize(const char *fileName);
	
		static bool LoadFromFile(const char* const fileName, char* const buff, unsigned int buff_size);
	
		static bool WriteToFile(const char* const fileName, char* const buff);

		static long long GetCurrentTimeMs();
	
	};
}

#endif //__HELPER_FUNC_H__
