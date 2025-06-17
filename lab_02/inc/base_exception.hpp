#ifndef __BASE_EXCEPTION_HPP__
#define __BASE_EXCEPTION_HPP__
#include "base_exception.h"
#include <cstdio>

BaseException::BaseException(const char *filename,
                             const char *classname,
                             int line,
                             const char *time,
                             const char *info) noexcept
{
    snprintf(err_info, err_msg_size, "%s, %s, %s, %d, %s, %s", 
        filename, classname, err_info, line, time, info);
}

const char *BaseException::what() const noexcept
{
    return err_info;
}

#endif
