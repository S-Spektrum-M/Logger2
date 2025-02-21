// this is intended to simply handling bc previously we were just using
// std::runtime_error for everything, this leads to bad management.

#pragma once
#include "LogEvent.hpp"
#include <exception>

namespace Spektral::Log {
class source_nullptr_excpetion : public std::exception {
public:
  source_nullptr_excpetion() {}
  const char *what() const noexcept override {
    return "Erorr initializing LogEvent because\n reccieved a nullptr for "
           "ISource *\n";
  }
};

class message_nullptr_excpetion : public std::exception {
public:
  message_nullptr_excpetion() {}
  const char *what() const noexcept override {
    return "Erorr initializing LogEvent because\n reccieved a nullptr for "
           "ISource *\n";
  }
};

class full_queue_exception : public std::exception {
private:
  LogLevel error_level;

public:
  full_queue_exception(const LogLevel &lvl) : error_level(lvl) {}
  const char *what() const noexcept override {
    switch (error_level) {
    case INFO:
      return "Error pushing to INFO level\n";
    case WARN:
      return "Error pushing to WARN level\n";
    case DEBUG:
      return "Error pushing to DEBUG level\n";
    case ERROR:
      return "Error pushing to ERROR level\n";
    }
  }
};

} // namespace Spektral::Log
