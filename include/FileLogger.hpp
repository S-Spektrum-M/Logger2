/**
 * @file FileLogger.hpp
 * @author Siddharth Mohanty
 * @namespace Spektral::Log
 */

#pragma once
#include "LogQueue.hpp"
#include <fstream>
#include <future>

namespace Spektral::Log {

/**
 * @brief A class for logging events to a file.
 *
 * This class provides a mechanism to efficiently log events to a file.  It uses
 * a queue to buffer log events and a background thread to write them to the
 * file, improving performance by minimizing blocking operations in the main
 * thread.
 */
class FileLogger {
public:
  /**
   * @brief Type alias for the log queue.
   */
  using log_t = LogQueue;

  /**
   * @brief Constructor that takes a file path to which logs will be written.
   *
   * @param file_path The file path to the file to log to.
   *
   * The constructor initializes an output stream used to write the log events.
   * This stream is used by a background thread to write the queued log events
   * to the specified file.
   *
   * @throws std::runtime_error If the file cannot be opened.
   *
   * Example:
   * @code
   * Logger("logs/network.log");
   * @endcode
   */
  explicit FileLogger(const std::string &file_path);

  /**
   * @brief Destructor.
   *
   * The destructor ensures that all pending log events are written to the file
   * before the logger is destroyed. It signals the background thread to stop
   * and waits for it to finish processing the queue.
   */
  ~FileLogger();

  /**
   * @brief Inserts a new LogEvent object into the log queue.
   *
   * @param event The LogEvent object to be inserted.  The event is moved into
   *              the queue.
   *
   * This function adds the provided LogEvent to the internal queue. The actual
   * writing to the file is handled by a background thread.  This allows the
   * application to continue execution without waiting for the file I/O to
   * complete.
   *
   * @note The inserted event is moved, i.e., it is no longer accessible in
   * its original location after this function call.
   */
  void insert(LogEvent &&event);

private:
  /// The output stream where log events are written to.
  std::shared_ptr<std::ofstream> _sink;

  /// Queue storing log events.
  log_t _log_queue;

  /// Atomic flag to control the background thread.
  std::atomic<bool> _can_continue;

  /**
   * @brief Starts the background logging thread.
   *
   * @param can_continue A reference to the atomic flag used to signal the
   *                     background thread to stop.
   * @return A future that can be used to wait for the background thread to
   *         terminate.
   *
   * This function creates and starts a background thread that continuously
   * monitors the log queue and writes events to the output stream.
   */
  [[nodiscard("Return is the value to stop the thread. Do not discard.")]]
  std::future<void> start_backend(std::atomic<bool> &can_continue);

  /// Future representing the background thread.
  std::future<void> _ref;
};

} // namespace Spektral::Log
