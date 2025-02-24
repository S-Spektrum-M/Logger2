#include "FileLogger.hpp"
#include <cmath>
#include <future>
#include <iostream>
#define LOG_MAX_SZ 100000000

namespace Spektral::Log {

FileLogger::FileLogger(const std::string &file_path)
    : _sink(std::make_shared<std::ofstream>(file_path)), _can_continue(true) {
  if (!_sink->is_open())
    throw std::runtime_error(std::format("Failed to open file: {}", file_path));
  _ref = start_backend(_can_continue);
}

FileLogger::~FileLogger() {
  _can_continue = false;
  _ref.get();
  _log_queue.clear();
  _sink->close();
}

void FileLogger::insert(LogEvent &&event) {
  _log_queue.emplace_back(std::make_shared<LogEvent>(std::move(event)));
}

std::future<void> FileLogger::start_backend(std::atomic<bool> &can_continue) {
  return std::async(std::launch::async, [this, &can_continue]() -> void {
    while (can_continue) {
      if (!_log_queue.empty()) {
        auto &front = _log_queue.front();
        if (front) {
          (*_sink) << front->operator std::string() << std::flush;
        }
        _log_queue.pop_front();
      }
    }

    while (!_log_queue.empty()) {
      auto &front = _log_queue.front();
      if (front) {
        (*_sink) << front->operator std::string() << std::flush;
      }
      _log_queue.pop_front();
    }
  });
}
} // namespace Spektral::Log
