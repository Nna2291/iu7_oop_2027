#pragma once

#include <collection/BaseException.h>
#include <cstring>

class InvalidIndexAccessException : public BaseException
{
  public:
    InvalidIndexAccessException(const char *filename, const char *classname, const char *method, int line)
        : BaseException(filename, classname, method, line, "List invalid index access")
    {
    }
};

class NodeAllocationException : public BaseException
{
  public:
    NodeAllocationException(const char *filename, const char *classname, const char *method, int line)
        : BaseException(filename, classname, method, line, "Failed to allocate memory for list node")
    {
    }
};

class EmptyListException : public BaseException
{
  public:
    EmptyListException(const char *filename, const char *classname, const char *method, int line)
        : BaseException(filename, classname, method, line, "Trying to get value in empty list")
    {
    }
};
