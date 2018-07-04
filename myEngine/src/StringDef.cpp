#include "StringDef.h"
#include <cstring>
#include <cstdarg>
#include <stdio.h>

namespace myEngine
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

int sprintf(char* buff, const int buff_len, const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	int r = 0;
#ifdef WIN32
	r = vsprintf_s(buff, buff_len, format, arg);
#else
	r = ::vsnprintf(buff, buff_len, format, arg);
#endif
	va_end(arg);

	buff[buff_len - 1] = 0;
	return r;
}

}// namespace myEngine