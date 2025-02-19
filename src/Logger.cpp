#include "Logger.hpp"
#include "LogCustomErrors.hpp"
#include <future>
#include <iostream>
#include <thread>
#define LOG_MAX_SZ 1000000

namespace Spektral::Log {

void Logger::insert_INFO(LogEvent &l) {
  if (_log_queue_INFO.size() == LOG_MAX_SZ)
    throw full_queue_exception(INFO);
  _log_queue_INFO.push_back(&l);
}

void Logger::insert_INFO(LogEvent &&l) {
  if (_log_queue_INFO.size() == LOG_MAX_SZ)
    throw full_queue_exception(INFO);
  _log_queue_INFO.push_back(new LogEvent(std::move(l)));
}

void Logger::insert_WARN(LogEvent &l) {
  if (_log_queue_WARN.size() == LOG_MAX_SZ)
    throw full_queue_exception(WARN);
  _log_queue_WARN.push_back(&l);
}

void Logger::insert_WARN(LogEvent &&l) {
  if (_log_queue_WARN.size() == LOG_MAX_SZ)
    throw full_queue_exception(WARN);
  _log_queue_WARN.push_back(new LogEvent(std::move(l)));
}

void Logger::insert_DEBUG(LogEvent &l) {
  if (_log_queue_DEBUG.size() == LOG_MAX_SZ)
    throw full_queue_exception(DEBUG);
  _log_queue_DEBUG.push_back(&l);
}

void Logger::insert_DEBUG(LogEvent &&l) {
  if (_log_queue_DEBUG.size() == LOG_MAX_SZ)
    throw full_queue_exception(DEBUG);
  _log_queue_DEBUG.push_back(new LogEvent(std::move(l)));
}

void Logger::insert_ERROR(LogEvent &l) {
  if (_log_queue_ERROR.size() == LOG_MAX_SZ)
    throw full_queue_exception(ERROR);
  _log_queue_ERROR.push_back(&l);
}

void Logger::insert_ERROR(LogEvent &&l) {
  if (_log_queue_ERROR.size() == LOG_MAX_SZ)
    throw full_queue_exception(ERROR);
  _log_queue_ERROR.push_back(new LogEvent(std::move(l)));
}

void Logger::insert(LogLevel lvl, LogEvent &event) {
  switch (lvl) {
  case INFO:
    insert_INFO(event);
    break;
  case WARN:
    insert_INFO(event);
    break;
  case DEBUG:
    insert_INFO(event);
    break;
  case ERROR:
    insert_INFO(event);
    break;
  }
}

void Logger::insert(LogLevel lvl, LogEvent &&event) {
  switch (lvl) {
  case INFO:
    insert_INFO(event);
    break;
  case WARN:
    insert_INFO(event);
    break;
  case DEBUG:
    insert_INFO(event);
    break;
  case ERROR:
    insert_INFO(event);
    break;
  }
}

std::future<void> Logger::start_backend(std::atomic<bool> &can_continue) {
  return std::async(std::launch::async, [this, &can_continue]() -> void {
    std::vector<log_t *> logs = {&_log_queue_INFO, &_log_queue_WARN,
                                 &_log_queue_DEBUG, &_log_queue_ERROR};
    while (can_continue) {
      for (const auto &log : logs) {
        if (!log->empty()) {
          auto front = log->front();
          if (front)
            _sink << front->operator std::string() << std::flush;
          log->pop_front();
        }
      }
    }

    while (!_log_queue_INFO.empty() || !_log_queue_WARN.empty() ||
           !_log_queue_DEBUG.empty() || !_log_queue_ERROR.empty()) {
      for (const auto &log : logs) {
        if (!log->empty()) {
          auto front = log->front();
          _sink << front->operator std::string() << std::flush;
          log->pop_front();
        }
      }
    }
  });
}
} // namespace Spektral::Log
