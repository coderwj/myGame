#include "HelperFunc.h"
#include "StringDef.h"

bool HelperFunc::convToWinPath(char * path)
{
	const unsigned int MAX_LEN = 2048;
	char temppath[MAX_LEN] = { '\0' };
	eml::strcpy(temppath, MAX_LEN, path);

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
	eml::strcpy(path, MAX_LEN, temppath);
	return false;
}