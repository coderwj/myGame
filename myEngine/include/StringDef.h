#ifndef __STRING_DEF_H__
#define __STRING_DEF_H__

#include <cstring>

namespace eml
{

#define strtohex(s) ::sretoul(s, NULL, 16)

void strcpy(char* dest, const int dest_size, const char* const src)
{
#ifdef WIN32
	strcpy_s(dest, dest_size, src);
#else
	::strncpy(dest, src, dest_size);
#endif
	dest[dest_size - 1] = 0;
}

void strncpy(char* dest, const int dest_size, const char* const src, const int copy_count)
{
#ifdef WIN32
	strncpy_s(dest, dest_size, src, copy_count);
#else
	int len = copy_count > dest_size - 1 ? dest_size - 1 : copy_count;
	::strncpy(dest, src, len);
#endif
	dest[dest_size - 1] = 0;
}

}// namespace eml

#endif //__STRING_DEF_H__