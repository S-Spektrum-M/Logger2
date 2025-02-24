#pragma once
#include "LogEvent.hpp"
#include <exception>

/**
 * @file LogExceptions.hpp
 * @namespace Spektral::Log
 * @brief Exception classes for log event handling.
 */

#pragma once

#include "LogEvent.hpp"
#include <exception>

namespace Spektral::Log {
/**
 * @class source_nullptr_excpetion
 * @brief Thrown when a nullptr is received for ISource* in LogEvent
 * initialization.
 */
class source_nullptr_exception : public std::exception {
public:
  /** Constructor. Initializes the exception object. */
  source_nullptr_exception() {}

  /**
   * @brief Returns the error message describing the cause of the exception.
   * @return A constant pointer to a null-terminated character string that
   * describes the exception's error message.
   */
  const char *what() const noexcept override {
    return "Error initializing LogEvent because\nreceived a nullptr for "
           "ISource *\n";
  }
};

/**
 * @class message_nullptr_exception
 * @brief Thrown when a nullptr is received for message in LogEvent
 * initialization.
 */
class message_nullptr_exception : public std::exception {
public:
  /** Constructor. Initializes the exception object. */
  message_nullptr_exception() {}

  /**
   * @brief Returns the error message describing the cause of the exception.
   * @return A constant pointer to a null-terminated character string that
   * describes the exception's error message.
   */
  const char *what() const noexcept override {
    return "Error initializing LogEvent because\nreceived a nullptr for "
           "ISource *\n";
  }
};

/**
 * @class full_queue_exception
 * @brief Thrown when an attempt is made to push a log event to a full queue.
 */
class full_queue_exception : public std::exception {
private:
  LogLevel error_level; ///< The log level associated with the exception.

public:
  /** Constructor. Initializes the exception object with a specific log level.
   */
  explicit full_queue_exception(const LogLevel &lvl) : error_level(lvl) {}

  /**
   * @brief Returns the error message describing the cause of the exception.
   * @return A constant pointer to a null-terminated character string that
   * describes the exception's error message based on the log level.
   */
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
    default:
      return "Error pushing to UNKNOWN level\n";
    }
  }
};
} // namespace Spektral::Log
