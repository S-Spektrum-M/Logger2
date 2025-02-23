#pragma once
#include "LogEvent.hpp"
#include <deque>
#include <fstream>
#include <future>

namespace Spektral::Log {

class FileLogger {
public:
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
  explicit FileLogger(std::string file_path);
  ~FileLogger();

  /**
   * @brief Functions to dynamically insert a new LogEvent object into the data
   * structure.
   *
   * @note The inserted event is moved, i.e., it is no longer accessible in
   * its original location after this function call.
   */
  void insert(LogEvent &&event);

private:
  /// The output stream where log events are written to.
  std::shared_ptr<std::ofstream> _sink;

  /// Queue storing INFO level log events.
  log_t _log_queue;

private:
  std::atomic<bool> _can_continue;
  /// @brief Start logging to the sink.
  [[nodiscard("Return is the value to stop the thread. Do not discard.")]]
  std::future<void> start_backend(std::atomic<bool> &can_continue);
  std::future<void> _ref;
};
} // namespace Spektral::Log
