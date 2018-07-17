#ifndef __STRING_DEF_H__
#define __STRING_DEF_H__

namespace myEngine
{

#define strtohex(s) ::sretoul(s, NULL, 16)

void strcpy(char* dest, const int dest_size, const char* const src);

void strncpy(char* dest, const int dest_size, const char* const src, const int copy_count);

int sprintf(char* buff, const int buff_len, const char* format, ...);

}// namespace myEngine

#endif //__STRING_DEF_H__