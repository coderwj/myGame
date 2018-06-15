#include "gltools.h"
#include "StringDef.h"
#include <iostream>

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
	char info[512] = {0};
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
		eml::sprintf(info, sizeof(info),"GL ERROR--> reason: \"%s\", file:%s, line:%d", error.c_str(), file, line);
		std::cout << info << std::endl;
        //std::cout << error.c_str() << " | " << file << " (" << line << ")" << std::endl;
        //exit(int(errorCode));
    }
    return errorCode;
}
