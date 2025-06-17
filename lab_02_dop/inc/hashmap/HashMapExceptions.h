#pragma once

#include "collection/BaseException.h"

class InvalidKeyException : public BaseException
{
  public:
    InvalidKeyException(const char *filename, const char *classname, const char *method, int line)
        : BaseException(filename, classname, method, line, "Invalid key access")
    {
    }
};

class InvalidBucketIndexException : public BaseException
{
  public:
    InvalidBucketIndexException(const char *filename, const char *classname, const char *method, int line)
        : BaseException(filename, classname, method, line, "Invalid bucket index")
    {
    }
};

class InvalidIterator : public BaseException
{
  public:
    InvalidIterator(const char *filename, const char *classname, const char *method, int line)
        : BaseException(filename, classname, method, line, "Invalid iterator")
    {
    }
};
