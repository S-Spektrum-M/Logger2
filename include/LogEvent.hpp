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
 * Defines the following (in increasing order of severity):
 * - INFO: Informational messages
 * - WARN: Warning messages
 * - DEBUG: Debugging messages
 * - ERROR: Error messages
 */

enum class LogLevel : char {
  INFO = 0,  ///< Informational messages
  WARN = 1,  ///< Warning messages
  DEBUG = 2, ///< Debugging messages
  ERROR = 3  ///< Error messages
};

/**
 * @brief Type alias for time points in the system clock.
 *
 * @note This can be changed to `std::chrono::high_resolution_clock` if needed.
 */
using std_clock = std::chrono::system_clock;
using std_time_t = std_clock::time_point;

/**
 * @class ISource
 * @brief Interface for log event sources.
 *
 * This interface defines the contract for objects that represent the source
 * of a log event.  Implementations of this interface should provide a way
 * to convert the source information to a string representation.
 */
class ISource {
public:
  /**
   * @brief Converts the source to a string representation.
   * @return String representation of the source.
   */
  virtual operator std::string() = 0;

  /**
   * @brief Virtual destructor.
   *
   * Ensures proper cleanup of derived classes.
   */
  virtual ~ISource() = default;
};

/**
 * @class IMessage
 * @brief Interface for log event messages.
 *
 * This interface defines the contract for objects that represent the message
 * of a log event. Implementations should provide a way to convert the message
 * to a string representation.
 */
class IMessage {
public:
  /**
   * @brief Converts the message to a string representation.
   * @return String representation of the message.
   */
  virtual operator std::string() = 0;

  /**
   * @brief Virtual destructor.
   *
   * Ensures proper cleanup of derived classes.
   */
  virtual ~IMessage() = default;
};

/**
 * @struct LogEvent
 * @brief Represents a log event with relevant details.
 *
 * This structure encapsulates all the information associated with a single
 * log event, including its severity level, timestamp, source, and message.
 */
struct LogEvent {
  LogLevel level;                    ///< Severity level of the log event.
  std_time_t time;                   ///< Timestamp of the log event.
  std::unique_ptr<ISource> source;   ///< Source of the log event.
  std::unique_ptr<IMessage> message; ///< Message of the log event.

  /**
   * @brief Constructs a LogEvent.
   * @param level The severity level of the log event.
   * @param source Pointer to the source of the event.  Ownership is
   * transferred.
   * @param message Pointer to the message of the event. Ownership is
   * transferred.
   */
  LogEvent(LogLevel level, std::unique_ptr<ISource> source,
           std::unique_ptr<IMessage> message);

  /**
   * @brief Move constructor.
   * @param other The LogEvent to move from.
   *
   * @throw message_nullptr_exception
   * @throw source_nullptr_exception
   */
  LogEvent(LogEvent &&other);

  /**
   * @brief Destructor.
   *
   * The unique pointers `source` and `message` will automatically delete the
   * owned objects when the `LogEvent` is destroyed.  No manual `delete` calls
   * are necessary.
   */
  ~LogEvent() = default; // No user-defined logic needed thanks to unique_ptr

  /**
   * @brief Converts the log event to a string representation.
   * @return String representation of the log event.
   */
  operator std::string();
};

} // namespace Spektral::Log
