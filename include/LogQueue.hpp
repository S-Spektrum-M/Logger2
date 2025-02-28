/**
 * @file LogQueue.hpp
 * @author Siddharth Mohanty
 * @namespace Spektral::Log
 */

#pragma once
#include "LogEvent.hpp"
#include <atomic>
#include <cstddef>

namespace Spektral::Log {
class LogQueue {
public:
  explicit LogQueue(size_t max_capacity = 5000000);
  ~LogQueue();
  void emplace_back(LogEvent &&log_e);
  std::shared_ptr<LogEvent> pop();
  void clear();
  bool empty();

private:
  [[maybe_unused]] std::atomic<size_t> _start_iterator;
  [[maybe_unused]] std::atomic<size_t> _end_iterator;
  size_t _max_capacity;
  LogEvent **arr;
  std::vector<std::shared_ptr<LogEvent>> _underlying;
};
}; // namespace Spektral::Log
