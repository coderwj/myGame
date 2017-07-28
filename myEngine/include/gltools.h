#ifndef __GLTOOLS_H__
#define __GLTOOLS_H__

#include "glew.h"

GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#endif