/**
 * @file log.hpp
 * @brief Logging system interfaces and structures.
 */

#pragma once
#include <chrono>
#include <memory>
#include <string>

namespace Spektral::Log {

/**
 * @enum LogLevel
 * @brief Defines different log severity levels.
 *
 * Defines the following(in increasing order of severity):
 * 1. INFO
 * 2. WARN
 * 3. DEBUG
 * 4. ERROR
 */
enum LogLevel : char {
  INFO = 0,  ///< Informational messages
  WARN = 1,  ///< Warning messages
  DEBUG = 2, ///< Debugging messages
  ERROR = 3  ///< Error messages
};

/**
 * @typedef std_time_t
 * @brief Alias for time points in the system clock.
 *
 * @note this can be changed to std::chrono::high_resoultion_clock if needed.
 */
using std_clock = std::chrono::system_clock;
using std_time_t = std_clock::time_point;

/**
 * @class ISource
 * @brief Interface for log event sources.
 */
class ISource {
public:
  /**
   * @brief Converts the source to a string representation.
   * @return String representation of the source.
   */
  virtual operator std::string_view() = 0;
  virtual ~ISource() = default;
};

/**
 * @class IMessage
 * @brief Interface for log event messages.
 */
class IMessage {
public:
  /**
   * @brief Converts the message to a string representation.
   * @return String representation of the message.
   */
  virtual operator std::string_view() = 0;
  virtual ~IMessage() = default;
};

/**
 * @struct LogEvent
 * @brief Represents a log event with relevant details.
 */
struct LogEvent {
  LogLevel level;                    ///< Severity level of the log event.
  std_time_t time;                   ///< Timestamp of the log event.
  std::unique_ptr<ISource> source;   ///< Source of the log event.
  std::unique_ptr<IMessage> message; ///< Message of the log event.

  /**
   * @brief Constructs a LogEvent.
   * @param source Pointer to the source of the event.
   * @param message Pointer to the message of the event.
   */
  LogEvent(LogLevel level, std::unique_ptr<ISource> source, std::unique_ptr<IMessage> message);
  LogEvent(LogEvent &&);

  /**
   * @brief Converts the log event to a string representation.
   * @return String representation of the log event.
   */
  operator std::string_view();
};

} // namespace Spektral::Log
