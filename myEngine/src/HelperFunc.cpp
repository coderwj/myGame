#include "HelperFunc.h"
#include "StringDef.h"

#include <cstdio>

namespace myEngine
{
	bool HelperFunc::convToWinPath(char * path)
	{
		const unsigned int MAX_LEN = 2048;
		char temppath[MAX_LEN] = { '\0' };
		myEngine::strcpy(temppath, MAX_LEN, path);
	
		char * prepath = temppath;
		for (size_t i = 0; i < MAX_LEN; i++)
		{
			if (*prepath == '/')
			{
				*path = '\\';
				path++;
				*path = '\\';
			}
			else
			{
				*path = *prepath;
				if (*prepath == '\0')
				{
					return true;
				}
			}
			prepath++;
			path++;
		}
		myEngine::strcpy(path, MAX_LEN, temppath);
		return false;
	}
	
	size_t HelperFunc::getFileSize(const char *fileName)
	{
	#ifdef WIN32
		FILE * fp = NULL;
		int r =	::fopen_s(&fp, fileName, "rb");
		if (0 != r)
		{
			return -1;
		}
	#else
		FILE * fp = ::fopen(fileName, "rb");
		if (NULL == fp)
		{
			return -1;
		}
	#endif
	
		::fseek(fp, 0, SEEK_END);
		size_t size = static_cast<size_t>(ftell(fp));
		::fseek(fp, 0, SEEK_SET);
		::fclose(fp);
	
		return size;
	}
	
	bool HelperFunc::LoadFromFile(const char* const fileName, char* const buff, unsigned int buff_size)
	{
	
	#ifdef WIN32
		FILE * fp = NULL;
		int r = ::fopen_s(&fp, fileName, "rb");
		if (0 != r)
		{
			return false;
		}
	#else
		FILE * fp = ::fopen(fileName, "rb");
		if (NULL == fp)
		{
			return false;
		}
	#endif
	
		::fread(buff, 1, buff_size, fp);
		::fclose(fp);
	
		buff[buff_size - 1] = 0;
		return true;
	}
	
	bool HelperFunc::WriteToFile(const char* const fileName, char* const buff)
	{
	#ifdef WIN32
		FILE * fp = NULL;
		int r = ::fopen_s(&fp, fileName, "rb");
		if (0 != r)
		{
			return false;
		}
	#else
		FILE * fp = ::fopen(fileName, "rb");
		if (NULL == fp)
		{
			return false;
		}
	#endif
		::fprintf(fp, "%s", buff);
		::fclose(fp);
	
		return true;
	}
}
