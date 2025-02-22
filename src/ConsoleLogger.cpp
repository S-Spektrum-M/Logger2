#include "ConsoleLogger.hpp"
#include "LogCustomErrors.hpp"
#include <cmath>
#include <iostream>
#define LOG_MAX_SZ 1000000

namespace Spektral::Log {
ConsoleLogger *ConsoleLogger::inst = nullptr;

ConsoleLogger::ConsoleLogger() { _ref = start_backend(_can_continue); }

ConsoleLogger &ConsoleLogger::get_inst() {
  if (!inst)
    inst = new ConsoleLogger();
  return *inst;
}
void ConsoleLogger::insert_INFO(LogEvent &l) {
  if (_stdout_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(INFO);
  _stdout_log.push_back(std::shared_ptr<LogEvent>{&l});
}

void ConsoleLogger::insert_INFO(LogEvent &&l) {
  if (_stdout_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(INFO);
  _stdout_log.push_back(std::make_shared<LogEvent>(std ::move(l)));
}

void ConsoleLogger::insert_WARN(LogEvent &l) {
  if (_stdout_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(WARN);
  _stdout_log.push_back(std::shared_ptr<LogEvent>{&l});
}

void ConsoleLogger::insert_WARN(LogEvent &&l) {
  if (_stdout_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(WARN);
  _stdout_log.push_back(std::make_shared<LogEvent>(std ::move(l)));
}

void ConsoleLogger::insert_DEBUG(LogEvent &l) {
  if (_stdout_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(DEBUG);
  _stdout_log.push_back(std::shared_ptr<LogEvent>{&l});
}

void ConsoleLogger::insert_DEBUG(LogEvent &&l) {
  if (_stdout_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(DEBUG);
  _stdout_log.push_back(std::make_shared<LogEvent>(std ::move(l)));
}

void ConsoleLogger::insert_ERROR(LogEvent &l) {
  if (_stderr_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(ERROR);
  _stderr_log.push_back(std::shared_ptr<LogEvent>{&l});
}

void ConsoleLogger::insert_ERROR(LogEvent &&l) {
  if (_stderr_log.size() == LOG_MAX_SZ)
    throw full_queue_exception(ERROR);
  _stderr_log.push_back(std::make_shared<LogEvent>(std ::move(l)));
}

std::future<void>
ConsoleLogger::start_backend(std::atomic<bool> &can_continue) {
  return std::async(std::launch::async, [this, &can_continue]() -> void {
    std::vector<log_t *> logs = {&_stdout_log, &_stderr_log};
    while (can_continue) {
      if (!_stdout_log.empty()) {
        auto &front = _stdout_log.front();
        if (front) {
          std::cout << front->operator std::string_view() << std::flush;
        }
        _stdout_log.pop_front();
      }
      if (!_stderr_log.empty()) {
        auto &front = _stderr_log.front();
        if (front) {
          std::cout << front->operator std::string_view() << std::flush;
        }
        _stderr_log.pop_front();
      }
    }

    while (!_stdout_log.empty() || !_stderr_log.empty()) {
      if (!_stdout_log.empty()) {
        auto &front = _stdout_log.front();
        if (front) {
          std::cout << front->operator std::string_view() << std::flush;
        }
        _stdout_log.pop_front();
      }
      if (!_stderr_log.empty()) {
        auto &front = _stderr_log.front();
        if (front) {
          std::cout << front->operator std::string_view() << std::flush;
        }
        _stderr_log.pop_front();
      }
    }
  });
}
} // namespace Spektral::Log
