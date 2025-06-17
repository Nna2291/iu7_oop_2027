#pragma once
#include "baseexception.h"

class ManagerException : public BaseException 
{
    public:
    ManagerException(const char *time, const char *filename,
                     const size_t line, const char *class_name,
                     const char *method_name,
                     const char *info = "Manager exception.")
        : BaseException(time, filename, line, class_name, method_name, info){};
};

class LoadManagerException : public ManagerException 
{
    public:
    LoadManagerException(const char *time, const char *filename,
                     const size_t line, const char *class_name,
                     const char *method_name,
                     const char *info = "Load manager exception.")
        : ManagerException(time, filename, line, class_name, method_name, info){};
};

class LoadManagerWrongDirectorException : public LoadManagerException {
    public:
        LoadManagerWrongDirectorException(const char *time, const char *filename,
                     const size_t line, const char *class_name,
                     const char *method_name,
                     const char *info = "Load manager exception. Wrong director.")
        : LoadManagerException(time, filename, line, class_name, method_name, info){};
};

class LoadManagerWrongReaderException : public LoadManagerException {
    public:
        LoadManagerWrongReaderException(const char *time, const char *filename,
                     const size_t line, const char *class_name,
                     const char *method_name,
                     const char *info = "Load manager exception. Wrong reader.")
        : LoadManagerException(time, filename, line, class_name, method_name, info){};
};
