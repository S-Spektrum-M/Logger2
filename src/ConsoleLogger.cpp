#include "ConsoleLogger.hpp"
#include <cmath>
#include <iostream>
#define LOG_MAX_SZ 100000000

namespace Spektral::Log {
ConsoleLogger *ConsoleLogger::inst = nullptr;

ConsoleLogger::ConsoleLogger(LogLevel min_level) : _min_level(min_level) {
  _ref = start_backend(_can_continue);
}

ConsoleLogger::~ConsoleLogger() {
  _can_continue = false;
  _ref.get();
  _stdout_log.clear();
  _stderr_log.clear();
  inst = nullptr;
}

ConsoleLogger &ConsoleLogger::get_inst(LogLevel min_level) {
  if (!inst)
    inst = new ConsoleLogger(min_level);
  return *inst;
}

void ConsoleLogger::insert(LogEvent &&l) {
  if (l.level < _min_level)
    return;
  switch (l.level) {
  case INFO:
  case WARN:
  case DEBUG:
    _stdout_log.emplace_back(std::move(l));
    break;
  case ERROR:
  default:
    _stderr_log.emplace_back(std::move(l));
    break;
  }
}

std::future<void>
ConsoleLogger::start_backend(std::atomic<bool> &can_continue) {
  return std::async(std::launch::async, [this, &can_continue]() -> void {
    std::vector<log_t *> logs = {&_stdout_log, &_stderr_log};
    while (can_continue) {
      if (!_stdout_log.empty()) {
        std::shared_ptr<LogEvent> &&front = _stdout_log.pop();
        std::cout << (std::string)(*front);
        void(std::async(std::launch::async, [&] { delete &front; }));
      }
      if (!_stderr_log.empty()) {
        auto &&front = _stderr_log.pop();
        std::cerr << (std::string)(*front);
        void(std::async(std::launch::async, [&] { delete &front; }));
      }
    }

    while (!_stdout_log.empty() || !_stderr_log.empty()) {
      if (!_stdout_log.empty()) {
        std::shared_ptr<LogEvent> &&front = _stdout_log.pop();
        std::cout << (std::string)(*front);
        void(std::async(std::launch::async, [&] { delete &front; }));
      }
      if (!_stderr_log.empty()) {
        auto &&front = _stderr_log.pop();
        std::cerr << (std::string)(*front);
        void(std::async(std::launch::async, [&] { delete &front; }));
      }
    }
  });
}
} // namespace Spektral::Log
