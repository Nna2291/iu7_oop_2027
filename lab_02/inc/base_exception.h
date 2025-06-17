#ifndef _BASE_EXCEPTION
#define _BASE_EXCEPTION

#include <exception>
#include <string.h>


class BaseException : public std::exception
{
    public:
        BaseException(const char *filename,
                    const char *classname,
                    int line,
                    const char *time,
                    const char *info = "ERROR") noexcept;

        const char *what() const noexcept override;

    protected:
        static const size_t err_msg_size = 1024;
        char err_info[err_msg_size];
};

#include "base_exception.hpp"

#endif
