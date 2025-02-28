/**
 * @file ConsoleLogger.hpp
 * @author Siddharth Mohanty
 * @namespace Spektral::Log
 */

#pragma once
#include "LogQueue.hpp"
#include <future>

namespace Spektral::Log {
class ConsoleLogger {
  // User Facing API
public:
  /**
   * @brief Get the singleton instance of ConsoleLogger.
   *
   * Retrieves the single instance of ConsoleLogger, initializing it if
   * necessary. The default minimum log level is set to WARN, but this can be
   * overridden by providing a different LogLevel in the argument.
   *
   * @param min_level The minimum LogLevel for messages to be logged. Default:
   * WARN.
   * @return A reference to the ConsoleLogger singleton instance.
   */
  static ConsoleLogger &get_inst(LogLevel min_level = LogLevel::WARN);
  /**
   * @brief Insert a log event into the logger's queue.
   *
   * Inserts a new LogEvent into either _stdout_log or _stderr_log based on
   * the severity of the message and the current minimum logging level.
   *
   * @param l A move-only reference to LogEvent that will be moved into the
   * internal queue.
   */
  void insert(LogEvent &&l);
  /**
   * @brief Destructor for ConsoleLogger.
   *
   * Cleans up any resources held by ConsoleLogger, including stopping
   * forcing log thread to finish logging.
   */
  ~ConsoleLogger();

private:
  /// Type alias for a deque of shared pointers to LogEvents to make
  /// implementations easier
  using log_t = LogQueue;

  /**
   * @brief Singleton instance pointer.
   *
   * A static pointer to the singleton instance of ConsoleLogger. This is used
   * internally by get_inst() to ensure thread-safe lazy initialization.
   */
  static ConsoleLogger *inst;
  /**
   * @brief Private constructor for ConsoleLogger.
   *
   * Initializes a new ConsoleLogger with an optional minimum log level.
   * The default minimum log level is WARN.
   *
   * @param min_level The minimum LogLevel to use. Default: WARN.
   */
  using enum LogLevel;
  explicit ConsoleLogger(LogLevel min_level = WARN);
  /// Queue of LogEvents intended for standard output
  log_t _stdout_log;
  /// Queue of LogEvents intended for standard error
  log_t _stderr_log;

private:
  /**
   * @brief Atomic variable indicating whether the logger can continue running.
   *
   * This atomic boolean is used to control background processing threads
   * and ensure they stop when the ConsoleLogger is destroyed or explicitly
   * stopped.
   */
  std::atomic<bool> _can_continue;
  /**
   * @brief Procedure to start the async log.
   *
   * @param can_continue An reference to a atomic bool, signaling wheter or not
   * the thread can continue.
   *
   * @return A handle on the async thread so that we can call get.
   */
  std::future<void> start_backend(std::atomic<bool> &can_continue);
  /**
   * @brief Private reference to the future returned by start_backend.
   *
   * This is used internally for managing and waiting on backend threads.
   */
  std::future<void> _ref;
  /// Minimum LogLevel at which messages will be recorded
  LogLevel _min_level;
};

}; // namespace Spektral::Log
