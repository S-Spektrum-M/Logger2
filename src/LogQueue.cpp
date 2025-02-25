#include "LogQueue.hpp"
namespace Spektral::Log {

LogQueue::LogQueue(size_t max_capacity)
    : _start_iterator(0), _end_iterator(0), _max_capacity(max_capacity) {
  arr = (std::unique_ptr<LogEvent> *)malloc(sizeof(std::unique_ptr<LogEvent>) *
                                            max_capacity);
}

LogQueue::~LogQueue() { free(arr); }

void LogQueue::emplace_back(LogEvent &&log_e) {
  if (_end_iterator == _max_capacity)
    throw std::exception();
  arr[_end_iterator++] = std::make_unique<LogEvent>(std::move(log_e));
}

LogEvent &LogQueue::pop() {
  if (_start_iterator++ == _end_iterator) {
    LogEvent &v = *(arr[_start_iterator].release());
    _start_iterator = 0;
    _end_iterator = 0;
    return v;
  }
  return *(arr[_start_iterator].release());
}

void LogQueue::clear() {
    while (_start_iterator != _end_iterator) {
        arr[_start_iterator++].reset(nullptr);
    }
    _start_iterator = 0;
    _end_iterator = 0;
}

bool LogQueue::empty() {
    return (_start_iterator == 0) && (_end_iterator == 0);
}
}; // namespace Spektral::Log
