#include "collection/BaseException.h"
#include <cstdio>

BaseException::BaseException(const char *filename, const char *classname, const char *method, int line, const char *desc = "Base Error") noexcept
{
    snprintf(message, MSG_LENGTH, "File: %s\nClass: %s\nMethod: %s\nLine: %i\nDescription: %s", filename,
             classname, method, line, desc);
	
}

const char *BaseException::what() const noexcept
{
    return message;
}
