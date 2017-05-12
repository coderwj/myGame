#include "HelperFunc.h"

bool HelperFunc::convToWinPath(char * path)
{
	const unsigned int MAX_LEN = 2048;
	char temppath[MAX_LEN] = { '\0' };
	strncpy(temppath, path, MAX_LEN);

	char * prepath = temppath;
	for (size_t i = 0; i < MAX_LEN; i++)
	{
		if (*prepath == '/')
		{
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
	strncpy(path, temppath, MAX_LEN);
	return false;
}