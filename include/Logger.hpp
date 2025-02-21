#pragma once
#include "LogEvent.hpp"
#include <deque>
#include <fstream>
#include <future>

namespace Spektral::Log {

class Logger {
public:
  // using log_t = std::deque<LogEvent *>;
  using log_t = std::deque<std::shared_ptr<LogEvent>>;
  /**
   * @brief Constructor that takes a stream to which logs will be written.
   *
   * @param sink: std::ofstream && The "sink"(where to log to) for the log
   *
   * The constructor initializes an output stream used to write the log events.
   * This stream can be a standard output or any other stream object like
   * std::ofstream. Example: Logger(std::cout);
   */
  explicit Logger(std::string file_path);
  ~Logger();

  /// @brief Inserts a log event with INFO severity level into the queue.
  ///
  /// This method inserts an INFO-level LogEvent into its corresponding queue.
  /// The event can be passed by reference or rvalue-reference for efficiency.
  void insert_INFO(LogEvent &l);

  /// @brief Inserts a log event with INFO severity level into the queue (rvalue
  /// version).
  ///
  /// This variant of `insert_INFO` allows passing a temporary LogEvent object,
  /// which is more efficient when dealing with short-lived objects.
  void insert_INFO(LogEvent &&l);

  /// @brief Inserts a log event with WARN severity level into the queue.
  ///
  /// Similar to `insert_INFO`, this method inserts a WARN-level LogEvent into
  /// its corresponding queue, either by reference or rvalue-reference.
  void insert_WARN(LogEvent &l);

  /// @brief Inserts a log event with WARN severity level into the queue (rvalue
  /// version).
  ///
  /// This variant is more efficient when dealing with temporary LogEvent
  /// objects.
  void insert_WARN(LogEvent &&l);

  /// @brief Inserts a log event with DEBUG severity level into the queue.
  ///
  /// Inserts a DEBUG-level LogEvent by reference or rvalue-reference, depending
  /// on the context in which it's called. Useful for debugging purposes.
  void insert_DEBUG(LogEvent &l);

  /// @brief Inserts a log event with DEBUG severity level into the queue
  /// (rvalue version).
  ///
  /// More efficient when dealing with temporary LogEvent objects that are not
  /// intended to be reused.
  void insert_DEBUG(LogEvent &&l);

  /// @brief Inserts a log event with ERROR severity level into the queue.
  ///
  /// Logs an ERROR-level event by reference or rvalue-reference, suitable for
  /// logging critical issues.
  void insert_ERROR(LogEvent &l);

  /// @brief Inserts a log event with ERROR severity level into the queue
  /// (rvalue version).
  ///
  /// Efficiently handles temporary LogEvent objects dedicated to error
  /// reporting.
  void insert_ERROR(LogEvent &&l);

  /// @brief Generic method for inserting a log event of any specified level.
  ///
  /// This function is a more generic way to insert log events, using an
  /// enumerated LogLevel to specify the severity and passing either a reference
  /// or rvalue-reference.
  void insert(LogLevel lvl, LogEvent &event);

  /// @brief Insert a log event (rvalue version).
  ///
  /// A variant of `insert` that allows for efficient handling of temporary
  /// LogEvents.
  void insert(LogLevel lvl, LogEvent &&event);

private:
  /// The output stream where log events are written to.
  std::shared_ptr<std::ofstream> _sink;

  /// Queue storing INFO level log events.
  log_t _log_queue_INFO;

  /// Queue storing WARN level log events.
  log_t _log_queue_WARN;

  /// Queue storing DEBUG level log events.
  log_t _log_queue_DEBUG;

  /// Queue storing ERROR level log events.
  log_t _log_queue_ERROR;

private:
  std::atomic<bool> _can_continue;
  /// @brief Start logging to the sink.
  [[nodiscard("Return is the value to stop the thread. Do not discard.")]]
  std::future<void> start_backend(std::atomic<bool> &can_continue);
  std::future<void> _ref;
};
} // namespace Spektral::Log
