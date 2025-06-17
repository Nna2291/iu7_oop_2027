#pragma once

#include <exception>

class BaseException : public std::exception
{
  public:
    BaseException(const char *filename, const char *classname, const char *method, int line, const char *desc) noexcept;

    const char *what() const noexcept override;

  protected:
    static constexpr size_t MSG_LENGTH = 512;
    char message[MSG_LENGTH];
};
